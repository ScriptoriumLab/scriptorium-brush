# Modian Brush

[![Modian IME Brush platform CI](https://github.com/ModianIME/Modian-win/actions/workflows/modian-ime-windows-platform-ci.yml/badge.svg)](https://github.com/ModianIME/Modian-win/actions/workflows/modian-ime-windows-platform-ci.yml)

```
=======================================================================================================================

 ,ggg, ,ggg,_,ggg,                                                               ,a8a,  ,ggg, ,ggg,_,ggg,     ,ggggggg,
dP""Y8dP""Y88P""Y8b                     8I                                      ,8" "8,dP""Y8dP""Y88P""Y8b  ,dP""""""Y8b
Yb, `88'  `88'  `88                     8I                                      d8   8bYb, `88'  `88'  `88  d8'    a  Y8
 `"  88    88    88                     8I   gg                                 88   88 `"  88    88    88  88     "Y8P'
     88    88    88                     8I   ""                                 88   88     88    88    88  `8baaaa
     88    88    88    ,ggggg,    ,gggg,8I   gg     ,gggg,gg   ,ggg,,ggg,       Y8   8P     88    88    88 ,d8P""""
     88    88    88   dP"  "Y8gggdP"  "Y8I   88    dP"  "Y8I  ,8" "8P" "8,      `8, ,8'     88    88    88 d8"
     88    88    88  i8'    ,8I i8'    ,8I   88   i8'    ,8I  I8   8I   8I 8888  "8,8"      88    88    88 Y8,
     88    88    Y8,,d8,   ,d8',d8,   ,d8b,_,88,_,d8,   ,d8b,,dP   8I   Yb,`8b,  ,d8b,      88    88    Y8,`Yba,,_____,
     88    88    `Y8P"Y8888P"  P"Y8888P"`Y88P""Y8P"Y8888P"`Y88P'   8I   `Y8  "Y88P" "Y8     88    88    `Y8  `"Y8888888

=======================================================================================================================
```

## 1. Introduction

This repository is now for the Windows version of Modian. Modian is a simple and easy-to-use Input Method Editor. It is designed to be user-friendly and easy to use. It is also designed to be lightweight and fast. Windows version of Modian is currently in development and is not yet complete.

## 2. Architecture

According to clean architecture, the architecture of Modian is designed like below:

![Modian Architecture V0.1.3](./docs/Modian%20Architecture%20V0.1.3.png)

---

## TODO
- [ ] design automation test strategies for Modian IME
- [ ] introduce e2e test
- [ ] introduce UI e2e test

---

## Tech debt

### C++ Code
- [x] Use `#pragma once` instead of `#ifndef` in header file
- [x] Move `class_factory` into `namespace modian::tsf`
- [ ] Refactor code in tsf
- [x] Introduce logger framework -- introduced `spdlog`
  - [x] Shutdown spdlog when Modian is unregistered from the system
  - [x] spdlog cannot log to `modian.log`
- [x] Remove redundant code in `util.h` and `registry_info.h`
- [x] Move load engine function to engine instead of in engine manager constructor
- [ ] Refactor to use unique_ptr of `tsf_key_event_service` in text service
- [ ] Packing open source Chinese character font into Modian

### Architecture
- [x] Introduce `app` layer to maintain `dll_main` and `server`
- [x] Introduce `manager` layer to manage different types of input engine
- [x] Add abstract of `Logger` in core
- [ ] Refactor `candidate` from `std::vector<std::wstring>` to `virtual_candidate` and `candidate : public virtual candidate`
- [ ] Refactor observer pattern to event channel pattern

### Thread Safety
- [ ] Implement Thread Safety Mechanisms: Introduce appropriate synchronization techniques to manage concurrent access to shared resources, preventing race conditions and ensuring data integrity.
- [ ] Designate Separate Read and Write Threads: Establish distinct threads for reading and writing operations to optimize performance and maintain data consistency during dictionary updates.
- [ ] Integrate SQLite for Dictionary Management: Transition to using SQLite for handling the dictionary, facilitating efficient storage, retrieval, and updates in a thread-safe manner.
- [ ] Develop a Strategy for Dynamic Dictionary Updates: Create a robust plan to handle real-time dictionary updates without disrupting ongoing input processing, ensuring that new entries are seamlessly incorporated.
- [ ] Conduct Comprehensive Testing: Perform thorough testing under various scenarios to identify and resolve potential threading issues, ensuring the stability and reliability of the input method.

### Input Event Handling
- [ ] Refactor TSF key event handling
  - Normalize `WPARAM`/`LPARAM` into a platform-independent **KeyEvent abstraction**.
  - Stop treating `WPARAM` as `wchar_t` (it's VK, not character).
  - Extract **VK**, **ScanCode**, **modifiers**, and **repeat** info from parameters.
  - Use `ToUnicodeEx` (or `WM_CHAR`) to translate into **UTF-8 text events**.
  - Route **control keys** (Backspace, Enter, arrows, etc.) via dedicated `handle_raw_keydown`.
  - Forward **text input** (UTF-8) via `handle_text_input`.
  - Ensure `OnTestKeyDown` mirrors `OnKeyDown` decision logic (no side effects).
  - Keep `OnKeyUp` and `OnTestKeyUp` consistent with the same abstraction.

## Bug
- [x] Cannot rebuild modian and remove modian directory after unregistering modian IME
- [x] Cannot show Chinese character candidates in ImGui framework
