# LearnCPP

A local, LeetCode-style C++ course for people coming from Python. 46 problems in 8 chapters:
types → references → pointers → heap memory & RAII → classes → STL → smart pointers & move semantics → lambdas/templates/optional.

## Run

```sh
python3 learn.py
```

That starts a server on http://127.0.0.1:8000 and opens your browser. Needs only Python 3 and `clang++`
(on macOS: `xcode-select --install`). No pip packages, no cmake.

Each problem is a short lesson plus a function or class to implement. **Run** tests your code without recording
anything; **Submit** (⌘↵) also counts the attempt and can mark the problem solved. Both compile your code with
`clang++ -std=c++17 -Wall -Wextra -fsanitize=address,undefined`, so memory bugs and undefined behavior show up as
readable reports instead of silent garbage. Hints are collapsed sections under the question; the reference solution is behind a two-click
reveal. The **Playground** runs any program with a `main()`.

**Tutor** (next to Hint) opens a side panel with a chat that sees the current problem, your code and your last result. It runs the
`claude` command line tool under your own Claude Code login, with all tools switched off, and is told to give hints
rather than answers unless you ask for the answer. Its instructions are `TUTOR_PROMPT` in `learn.py`; the model is
`TUTOR_MODEL`. Chats are kept until the page reloads.

Progress (solved state, your in-progress code, revealed hints) autosaves to `progress.json` next to this file.
Delete that file to start over.

If this folder is a git repo, every accepted **Submit** is also committed and pushed in the background: your code goes
to `submissions/<chapter>/<problem>.cpp` with a message like `Average of Three: Accepted (7/7)`. Failed submissions
are not committed. Git failures (offline, no remote) print one line in the terminal and never affect the submission.

## Layout

```
learn.py            server + compile/run pipeline (stdlib only)
check_problems.py   content self-check: every solution passes, every starter fails
test_learn.py       pipeline self-check (compile error, crash, timeout, output cap, ...)
harness/            check.h (CHECK_* macros), tracked.h (lifetime-counting helper used by memory chapters)
problems/<ch>/<p>/  meta.json, problem.md, starter.cpp, solution.cpp, test.cpp
web/                index.html, app.js, style.css, vendor/ (CodeMirror 5, marked; vendored so it works offline)
```

## Adding a problem

Copy any problem directory, edit the five files, run `python3 check_problems.py <dir-fragment>`.
`test.cpp` always starts with `#include "check.h"`, `#include "tracked.h"`, `#include "solution.cpp"` and defines
`void run_tests()`.
