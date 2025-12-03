# Contributing to ClockTimer

Thank you for your interest in contributing to **ClockTimer**!  
This project is intentionally small and focused, so contributions that keep the library clean, reliable, and easy to understand are especially welcome.

Please read the guidelines below before submitting changes.

---

## 1. How to contribute

You can contribute in several ways:

- Reporting bugs  
- Proposing improvements or new features  
- Submitting pull requests  
- Improving documentation or examples

Before starting work on a significant change, please **open an Issue** to discuss it first.  
This helps avoid duplicated work or features that don't fit the project’s direction.

---

## 2. Development workflow

### Clone the repository

```sh
git clone https://github.com/AlexZappy/ClockTimer.git
cd ClockTimer
```

## Make your changes on a feature branch

```sh
git checkout -b feature/my-new-feature
```

Commit using clear and descriptive messages.

## 3. Code style guidelines

To keep the code consistent:

- Use clean and readable C++ (C++11 level features only).

- Avoid dynamic memory or heavy abstractions (the library targets small MCUs).

- Keep `.h` files minimal and self-explanatory.

- Respect the existing naming style:

- `camelCase` for methods and variables

- `_leadingUnderscore` for private members

- Add inline comments where logic might not be obvious.

## 4. Testing your changes

ClockTimer is validated using PlatformIO CI.
Before submitting a pull request, please:

1. Build the examples on at least one board (e.g. Arduino Uno or ESP32).

2. Ensure that:

    - Both basic_clock_demo and multi_clock_demo compile.

    - No warnings are introduced.

3. Do not introduce delay() or blocking patterns.

If you add a new example, please keep it simple and well-documented.

---

## 5. Documentation updates

If you add new features or modify behavior:

- Update README.md

- Update docs/roadmap.md if the change affects future plans

- Update CHANGELOG.md

Documentation clarity is considered part of code quality.

---

## 6. Pull Request checklist

Before submitting, please make sure:

- Your branch builds successfully
- No unnecessary files are committed (no .vscode, no build folders, etc.)
- Examples compile
- Documentation is updated
- The PR description clearly explains the change and motivation

## 7. Licensing

By contributing, you agree that your code will be released under the same license as this project:
**GNU GPL v3.0.**

If you want to propose code from external sources, ensure it is _GPL-compatible._

## 8. Need help?

If something is unclear, feel free to open an Issue and ask.
The project aims to stay beginner-friendly and maintainable.
