#!/usr/bin/env python3
"""Content self-check. For every problem: the reference solution passes all tests,
the starter does NOT (compile error, failing case, crash or timeout), and metadata is complete.

    python3 check_problems.py            # all problems
    python3 check_problems.py 03-        # only ids containing "03-"
"""
import json
import sys
from concurrent.futures import ThreadPoolExecutor

import learn


def check(pid):
    d = learn.problem_dir(pid)
    problems = []
    meta = json.loads((d / "meta.json").read_text())
    for key in ("title", "difficulty"):
        if not meta.get(key):
            problems.append(f"meta.json missing {key}")
    if meta.get("difficulty") not in ("easy", "medium", "hard"):
        problems.append(f"bad difficulty {meta.get('difficulty')!r}")
    if len(meta.get("hints", [])) < 3:
        problems.append(f"only {len(meta.get('hints', []))} hints")
    for f in ("problem.md", "starter.cpp", "solution.cpp", "test.cpp"):
        if not (d / f).exists() or not (d / f).read_text().strip():
            problems.append(f"{f} missing or empty")
    if problems:
        return pid, problems, "", ""

    sol = learn.run_problem(pid, (d / "solution.cpp").read_text())
    if not sol["compile_ok"]:
        problems.append("solution does not compile:\n" + sol["compile_output"])
    elif not sol["solved"]:
        failed = [c["name"] + ": " + c["detail"] for c in sol["cases"] if not c["pass"]]
        problems.append(f"solution fails {sol['passed']}/{sol['total']} crashed={sol['crashed']} timed_out={sol['timed_out']}\n  "
                        + "\n  ".join(failed) + ("\n" + sol["runtime_error"] if sol["runtime_error"] else ""))
    elif sol["total"] < 3:
        problems.append(f"solution has only {sol['total']} test cases")
    sol_note = f"{sol['passed']}/{sol['total']}" if sol["compile_ok"] else "CE"
    if sol["compile_ok"] and sol["compile_output"]:
        sol_note += " (warnings)"

    st = learn.run_problem(pid, (d / "starter.cpp").read_text())
    if st["solved"]:
        problems.append("STARTER PASSES ALL TESTS")
    if not st["compile_ok"]:
        st_note = "CE"
    else:
        st_note = f"{st['passed']}/{st['total']}" + (" crash" if st["crashed"] else "") + (" timeout" if st["timed_out"] else "")
    return pid, problems, sol_note, st_note


def main():
    flt = sys.argv[1] if len(sys.argv) > 1 else ""
    ids = [i for i in learn.all_ids() if flt in i]
    if not ids:
        raise SystemExit("no problems found")
    with ThreadPoolExecutor(max_workers=6) as ex:
        results = list(ex.map(check, ids))
    bad = 0
    print(f"{'problem':48} {'solution':16} starter")
    for pid, problems, sol_note, st_note in results:
        flag = "FAIL" if problems else "ok  "
        print(f"{flag} {pid:43} {sol_note:16} {st_note}")
        for p in problems:
            print("     - " + p.replace("\n", "\n       "))
        bad += bool(problems)
    print(f"\n{len(ids) - bad}/{len(ids)} problems ok")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
