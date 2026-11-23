#!/usr/bin/env python3
"""Self-check for the compile/run pipeline in learn.py. Run: python3 test_learn.py"""
import time
import learn

PID = "01-basics/01-digit-sum"
SOL = open("problems/01-basics/01-digit-sum/solution.cpp").read()

r = learn.run_problem(PID, SOL)
assert r["solved"] and r["passed"] == r["total"] == 5, r

r = learn.run_problem(PID, "int digit_sum(int) { return 0; }")
assert not r["solved"] and r["passed"] == 1 and r["total"] == 5, r
assert "expected 7, got 0" in r["cases"][1]["detail"], r["cases"][1]

r = learn.run_problem(PID, "int digit_sum(int n) { return n +; }")
assert not r["compile_ok"] and "solution.cpp:1" in r["compile_output"], r

r = learn.run_problem(PID, "int digit_sum(int n) { int* p = new int(n); delete p; return *p; }")
assert r["crashed"] and "heap-use-after-free" in r["runtime_error"], r

r = learn.run_problem(PID, "int digit_sum(int n) { return n * 1000000000 * 1000; }")
assert r["crashed"] and "signed integer overflow" in r["runtime_error"], r

t0 = time.time()
r = learn.run_problem(PID, "int digit_sum(int n) { while (n >= 0) {} return n; }")
assert r["timed_out"] and not r["solved"] and time.time() - t0 < learn.RUN_TIMEOUT + 3, r

t0 = time.time()
r = learn.run_problem(PID, '#include <iostream>\nint digit_sum(int n) { while (true) std::cout << "x\\n"; return n; }')
assert not r["solved"] and time.time() - t0 < learn.RUN_TIMEOUT + 3, r
assert len(r["program_output"]) <= learn.OUTPUT_CAP + 64, len(r["program_output"])

r = learn.run_problem(PID, '#include <iostream>\nint digit_sum(int n) { std::cout << "debug " << n << "\\n"; return 0; }')
assert r["program_output"].startswith("debug 0") and r["total"] == 5, r

r = learn.run_playground('#include <iostream>\nint main() { std::cout << "hi\\n"; return 3; }')
assert r["compile_ok"] and r["stdout"] == "hi\n" and r["exit_code"] == 3, r

cases, out, summary = learn.parse_harness_output("noise\n@@CASE\ta\tPASS\t\nmore\n@@CASE\tb\tFAIL\texpected 1, got 2\n@@SUMMARY\t1\t2\n")
assert [c["pass"] for c in cases] == [True, False] and out == "noise\nmore" and summary == (1, 2)

try:
    learn.problem_dir("../../etc/passwd")
    raise AssertionError("traversal not rejected")
except FileNotFoundError:
    pass

print("test_learn.py: all checks passed")
