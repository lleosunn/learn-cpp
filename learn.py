#!/usr/bin/env python3
"""LearnCPP — a local, LeetCode-style C++ tutorial.

    python3 learn.py            # starts http://127.0.0.1:8000 and opens your browser
    python3 learn.py --port 9000 --no-browser

Stdlib only. Compiles your code with clang++ + AddressSanitizer/UBSan, runs the
problem's tests, and autosaves progress to progress.json next to this file.
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import tempfile
import threading
import traceback
import webbrowser
from datetime import datetime
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import unquote

ROOT = Path(__file__).resolve().parent
PROBLEMS = ROOT / "problems"
HARNESS = ROOT / "harness"
WEB = ROOT / "web"
PROGRESS_FILE = ROOT / "progress.json"
SUBMISSIONS = ROOT / "submissions"
GIT_LOCK = threading.Lock()

CXX = os.environ.get("CXX", "clang++")
CXXFLAGS = ["-std=c++17", "-Wall", "-Wextra", "-g", "-O0", "-fno-omit-frame-pointer",
            "-fsanitize=address,undefined", "-fno-sanitize-recover=all", f"-I{HARNESS}"]
RUN_ENV = dict(os.environ,
               ASAN_OPTIONS="detect_leaks=0:detect_stack_use_after_return=1:halt_on_error=1:color=never:hard_rss_limit_mb=1024",
               UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1:color=never")
COMPILE_TIMEOUT = 60
RUN_TIMEOUT = 5
OUTPUT_CAP = 64 * 1024          # bytes of stdout/stderr we read back
# ponytail: resource caps are ulimit -f (1 MB files) + ulimit -t (CPU) + ASan hard_rss_limit; no cgroups/sandbox.
RUN_WRAPPER = ["/bin/sh", "-c", 'ulimit -f 2048; ulimit -t 20; exec "$0"']

PID_RE = re.compile(r"^\d{2}-[a-z0-9-]+/\d{2}-[a-z0-9-]+$")

PLAYGROUND_DEFAULT = """#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "Hello, C++!\\n";
    return 0;
}
"""


# ----------------------------------------------------------------------------- problems

def load_catalog():
    """[{dir, title, blurb, problems: [{id, title, difficulty}]}] in directory order."""
    chapters = []
    for ch in sorted(p for p in PROBLEMS.iterdir() if p.is_dir()):
        info = json.loads((ch / "chapter.json").read_text()) if (ch / "chapter.json").exists() else {}
        probs = []
        for pd in sorted(p for p in ch.iterdir() if p.is_dir() and (p / "meta.json").exists()):
            meta = json.loads((pd / "meta.json").read_text())
            probs.append({"id": f"{ch.name}/{pd.name}", "title": meta.get("title", pd.name),
                          "difficulty": meta.get("difficulty", "easy")})
        chapters.append({"dir": ch.name, "title": info.get("title", ch.name), "blurb": info.get("blurb", ""),
                         "problems": probs})
    return chapters


def all_ids():
    return [p["id"] for ch in load_catalog() for p in ch["problems"]]


def problem_dir(pid):
    if not PID_RE.match(pid):
        raise FileNotFoundError(pid)
    d = PROBLEMS / pid
    if not (d / "meta.json").exists():
        raise FileNotFoundError(pid)
    return d


# ----------------------------------------------------------------------------- progress

class Progress:
    def __init__(self, path):
        self.path = path
        self.lock = threading.Lock()
        self.data = {"version": 1, "last_problem": None, "playground_code": None, "problems": {}}
        if path.exists():
            try:
                self.data.update(json.loads(path.read_text()))
            except (OSError, ValueError):
                shutil.copy(path, path.with_suffix(".json.corrupt"))

    def entry(self, pid):
        return self.data["problems"].setdefault(
            pid, {"status": "untouched", "code": None, "attempts": 0, "hints_revealed": 0, "solved_at": None})

    def save(self):
        with self.lock:
            tmp = self.path.with_suffix(".json.tmp")
            tmp.write_text(json.dumps(self.data, indent=1))
            os.replace(tmp, self.path)


progress = Progress(PROGRESS_FILE)


# ----------------------------------------------------------------------------- compile & run

def _read_capped(path):
    with open(path, "rb") as f:
        data = f.read(OUTPUT_CAP + 1)
    text = data[:OUTPUT_CAP].decode("utf-8", "replace")
    if len(data) > OUTPUT_CAP:
        text += "\n… [output truncated]"
    return text


def compile_and_run(main_source, extra_files):
    """Compile main_source (name -> text) with extra_files in a temp dir and run it.
    Returns dict(compile_ok, compile_output, timed_out, exit_code, stdout, stderr)."""
    work = Path(tempfile.mkdtemp(prefix="learncpp_"))
    try:
        for name, text in extra_files.items():
            (work / name).write_text(text)
        (work / main_source[0]).write_text(main_source[1])
        try:
            c = subprocess.run([CXX, *CXXFLAGS, main_source[0], "-o", "prog"], cwd=work,
                               capture_output=True, text=True, timeout=COMPILE_TIMEOUT)
        except subprocess.TimeoutExpired:
            return {"compile_ok": False, "compile_output": "Compiler timed out.", "timed_out": True,
                    "exit_code": None, "stdout": "", "stderr": ""}
        res = {"compile_ok": c.returncode == 0, "compile_output": c.stderr.strip(), "timed_out": False,
               "exit_code": None, "stdout": "", "stderr": ""}
        if not res["compile_ok"]:
            return res
        with open(work / "out.txt", "wb") as out, open(work / "err.txt", "wb") as err:
            try:
                r = subprocess.run([*RUN_WRAPPER, str(work / "prog")], cwd=work, stdin=subprocess.DEVNULL,
                                   stdout=out, stderr=err, env=RUN_ENV, timeout=RUN_TIMEOUT)
                res["exit_code"] = r.returncode
            except subprocess.TimeoutExpired:
                res["timed_out"] = True
        res["stdout"] = _read_capped(work / "out.txt")
        res["stderr"] = _read_capped(work / "err.txt")
        return res
    finally:
        shutil.rmtree(work, ignore_errors=True)


def parse_harness_output(stdout):
    cases, output, summary = [], [], None
    for line in stdout.split("\n"):
        if line.startswith("@@CASE\t"):
            parts = line.split("\t", 3) + ["", ""]
            cases.append({"name": parts[1], "pass": parts[2] == "PASS", "detail": parts[3]})
        elif line.startswith("@@SUMMARY\t"):
            p, t = line.split("\t")[1:3]
            summary = (int(p), int(t))
        else:
            output.append(line)
    return cases, "\n".join(output).strip(), summary


def trim_sanitizer(stderr, max_lines=60):
    lines = stderr.strip().split("\n")
    # drop the noisy "SUMMARY:"-after part and shadow-byte legend of ASan reports
    cut = next((i for i, l in enumerate(lines) if l.startswith("Shadow bytes around")), len(lines))
    lines = lines[:min(cut, max_lines)]
    return "\n".join(lines).strip()


def run_problem(pid, code):
    d = problem_dir(pid)
    r = compile_and_run(("test.cpp", (d / "test.cpp").read_text()), {"solution.cpp": code})
    result = {"compile_ok": r["compile_ok"], "compile_output": r["compile_output"], "timed_out": r["timed_out"],
              "crashed": False, "cases": [], "program_output": "", "runtime_error": "", "passed": 0, "total": 0,
              "solved": False}
    if not r["compile_ok"]:
        return result
    cases, output, summary = parse_harness_output(r["stdout"])
    result.update(cases=cases, program_output=output, passed=sum(c["pass"] for c in cases), total=len(cases))
    if summary is None and not r["timed_out"]:
        result["crashed"] = True
        result["runtime_error"] = trim_sanitizer(r["stderr"]) or f"Program exited with code {r['exit_code']} before finishing the tests."
    elif r["stderr"].strip():
        result["runtime_error"] = trim_sanitizer(r["stderr"])
    if summary is not None:
        result["passed"], result["total"] = summary
    result["solved"] = (summary is not None and result["total"] > 0 and result["passed"] == result["total"]
                        and not result["crashed"] and not result["timed_out"])
    return result


def run_playground(code):
    r = compile_and_run(("main.cpp", code), {})
    r["stderr"] = trim_sanitizer(r["stderr"], max_lines=80)
    return r


# ----------------------------------------------------------------------------- api

def api_problems():
    cat = load_catalog()
    solved = 0
    total = 0
    for ch in cat:
        for p in ch["problems"]:
            p["status"] = progress.entry(p["id"])["status"]
            solved += p["status"] == "solved"
            total += 1
    return {"chapters": cat, "last_problem": progress.data["last_problem"], "solved": solved, "total": total}


def api_problem(pid):
    d = problem_dir(pid)
    meta = json.loads((d / "meta.json").read_text())
    e = progress.entry(pid)
    starter = (d / "starter.cpp").read_text()
    progress.data["last_problem"] = pid
    progress.save()
    return {"id": pid, "meta": meta, "problem_md": (d / "problem.md").read_text(), "starter": starter,
            "test_src": (d / "test.cpp").read_text(), "code": e["code"] if e["code"] is not None else starter,
            "hints_revealed": e["hints_revealed"], "status": e["status"], "attempts": e["attempts"]}


def api_solution(pid):
    return {"solution": (problem_dir(pid) / "solution.cpp").read_text()}


def api_save(body):
    pid = body["id"]
    d = problem_dir(pid)
    e = progress.entry(pid)
    if "code" in body:
        e["code"] = body["code"]
        if e["status"] == "untouched" and body["code"].strip() != (d / "starter.cpp").read_text().strip():
            e["status"] = "attempted"
    if "hints_revealed" in body:
        e["hints_revealed"] = max(e["hints_revealed"], int(body["hints_revealed"]))
    progress.save()
    return {"ok": True, "status": e["status"]}


def git_record(pid, code, result):
    """Commit an accepted submission (the code as submissions/<pid>.cpp, plus progress.json) and push it.

    Runs in the background and is best effort: if this is not a git repo, or the push fails,
    it prints one line and the submission is unaffected. An unpushed commit goes up with the next push.
    """
    title = json.loads((problem_dir(pid) / "meta.json").read_text())["title"]
    src = SUBMISSIONS / f"{pid}.cpp"

    def work():
        with GIT_LOCK:  # one git process at a time, commits in submission order
            try:
                src.parent.mkdir(parents=True, exist_ok=True)
                src.write_text(code)
                paths = ["--", str(src), str(PROGRESS_FILE)]
                for args in (["add", *paths], ["commit", "-q", "-m", f"{title}: Accepted ({result['passed']}/{result['total']})", *paths], ["push", "-q"]):
                    subprocess.run(["git", "-C", str(ROOT), *args], check=True, capture_output=True, text=True,
                                   timeout=30, env=dict(os.environ, GIT_TERMINAL_PROMPT="0"))
            except (subprocess.SubprocessError, OSError) as err:
                print(f"git: {(getattr(err, 'stderr', None) or str(err)).strip()}")

    # ponytail: daemon thread, so Ctrl+C right after a submit can drop that push; it goes up with the next one.
    threading.Thread(target=work, daemon=True).start()


def api_run(body):
    pid, code = body["id"], body["code"]
    result = run_problem(pid, code)
    e = progress.entry(pid)
    e["code"] = code
    e["attempts"] += 1
    if result["solved"]:
        if e["status"] != "solved":
            e["solved_at"] = datetime.now().isoformat(timespec="seconds")
        e["status"] = "solved"
    elif e["status"] == "untouched":
        e["status"] = "attempted"
    progress.save()
    if result["solved"]:
        git_record(pid, code, result)
    ids = all_ids()
    nxt = ids[ids.index(pid) + 1] if pid in ids and ids.index(pid) + 1 < len(ids) else None
    result.update(status=e["status"], next_id=nxt)
    return result


def api_playground(body):
    if "code" in body:
        progress.data["playground_code"] = body["code"]
        progress.save()
    if body.get("run"):
        return run_playground(body["code"])
    return {"ok": True}


MIME = {".html": "text/html; charset=utf-8", ".js": "application/javascript", ".css": "text/css",
        ".json": "application/json", ".svg": "image/svg+xml", ".png": "image/png", ".ico": "image/x-icon"}


class Handler(BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):  # quiet; errors still surface as 500 JSON
        pass

    def _json(self, obj, status=200):
        data = json.dumps(obj).encode()
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def _static(self, rel):
        path = (WEB / rel).resolve()
        if not str(path).startswith(str(WEB.resolve())) or not path.is_file():
            self.send_error(404)
            return
        data = path.read_bytes()
        self.send_response(200)
        self.send_header("Content-Type", MIME.get(path.suffix, "application/octet-stream"))
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-cache")
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        try:
            path = unquote(self.path.split("?")[0])
            if path == "/":
                return self._static("index.html")
            if path.startswith("/web/"):
                return self._static(path[len("/web/"):])
            if path == "/api/problems":
                return self._json(api_problems())
            if path.startswith("/api/problem/"):
                return self._json(api_problem(path[len("/api/problem/"):]))
            if path.startswith("/api/solution/"):
                return self._json(api_solution(path[len("/api/solution/"):]))
            if path == "/api/playground":
                return self._json({"code": progress.data["playground_code"] or PLAYGROUND_DEFAULT})
            self.send_error(404)
        except FileNotFoundError as e:
            self._json({"error": f"not found: {e}"}, 404)
        except Exception:
            self._json({"error": traceback.format_exc()}, 500)

    def do_POST(self):
        try:
            n = int(self.headers.get("Content-Length", 0))
            body = json.loads(self.rfile.read(n) or b"{}")
            routes = {"/api/save": api_save, "/api/run": api_run, "/api/playground": api_playground}
            fn = routes.get(self.path)
            if not fn:
                return self.send_error(404)
            self._json(fn(body))
        except FileNotFoundError as e:
            self._json({"error": f"not found: {e}"}, 404)
        except Exception:
            self._json({"error": traceback.format_exc()}, 500)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", type=int, default=8000)
    ap.add_argument("--no-browser", action="store_true")
    args = ap.parse_args()
    if shutil.which(CXX) is None:
        raise SystemExit(f"{CXX} not found. On macOS run: xcode-select --install")
    server = None
    for port in range(args.port, args.port + 20):
        try:
            server = ThreadingHTTPServer(("127.0.0.1", port), Handler)
            break
        except OSError:
            continue
    if server is None:
        raise SystemExit("no free port")
    url = f"http://127.0.0.1:{server.server_port}/"
    print(f"LearnCPP running at {url}  (Ctrl+C to stop)")
    if not args.no_browser:
        threading.Timer(0.4, webbrowser.open, [url]).start()
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nbye")


if __name__ == "__main__":
    main()
