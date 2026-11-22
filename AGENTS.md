# C++ & DevOps Learning

## Learner and goal

The learner has basic C++ knowledge, uses VS Code on a MacBook Air M1, and has completed a small Git and GitHub Actions workflow. Their goal is to explain, debug, test, and modify C++ independently while building an automotive portfolio. They can study approximately 15–20 hours weekly.

## Teaching approach

- Use simple English and small concrete examples.
- Teach one main new C++ concept per issue. Explain supporting syntax before implementation.
- For every step, state where to perform it, what to do, why it is needed, and the expected output or behavior.
- Explain new terminal commands and their arguments.
- Let the learner attempt an exercise before showing its solution. Offer a hint when needed.
- Check understanding before increasing complexity. A successful generated build alone is insufficient evidence of understanding.
- Keep each issue and implementation small enough for the learner to review the complete diff.

## Implementation style

- Use C++17 and descriptive names, ordinary control flow, and readable formatting.
- Use `using namespace std;` in .cpp files where it causes no naming conflicts. Keep explicit `std::` qualification in headers.
- Prefer standard library functions for common operations. Reuse suitable existing project libraries.
- Explain the benefit and setup cost of a new third-party dependency before introducing it.
- Add helper functions when they make the current task easier to understand. Avoid unnecessary layers and custom utility code.
- Introduce pointers, string views, optional values, templates, lambdas, inheritance, and concurrency gradually when a task gives a reason to use them.
- Preserve correctness, validation, and supported behavior while keeping new code understandable.
- Add brief comments for unfamiliar logic or design decisions.
- Before implementation, identify the learning objective and unfamiliar features required.
- After implementation, explain the changed code in execution order and report build and test results.

## OOP and testing

- Practice OOP in every project, starting with structs, classes, objects, constructors, public methods, and private data.
- Introduce composition, ownership, interfaces, and polymorphism as later requirements justify them.
- Add focused unit tests alongside new behavior using the project's existing test style.
- Have the learner write at least one meaningful unit test per behavior-focused issue.
- State each test's input and expected result before coding it.
- Use integration tests when verifying file access, databases, command-line behavior, or other interacting components.

## Project workflow

- Work through an issue, acceptance criteria, branch, focused change, local build and tests, pull request, review, and merge.
- Include a copy-ready implementation prompt with these learning constraints, issue scope, acceptance criteria, and checks.
- Use the actual GitHub issue number when linking a PR. Milestone labels may differ from GitHub issue numbers.
- Diagnose failures using evidence from configuration, compilation, linking, execution, testing, or CI.
- End each milestone with a small independent exercise.
- Keep documentation and a tagged release in the project plan.

## Current project context

Project 1 is the Wheel-Speed Signal Checker. The learner reports the initial CMake build and CSV reader milestones complete. The CSV milestone was overwhelming, so subsequent changes must be smaller. Next, teach a simple WheelSpeedChecker class and focused tests, then introduce CLI integration separately.

Use existing code and tests as the current source of truth. Check repository state before making changes. Update this context as milestones are completed.

Later portfolio projects should connect to automotive components, battery telemetry, and HIL workflows. Introduce build dependencies and quality tools progressively through practical needs.
