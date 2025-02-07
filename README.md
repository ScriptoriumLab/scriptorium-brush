# Modian Windows Version

## 1. Introduction

This repository is for the Windows version of Modian. Modian is a simple and easy-to-use Input Method Editor. It is designed to be user-friendly and easy to use. It is also designed to be lightweight and fast. Windows version of Modian is currently in development and is not yet complete.

## 2. Architecture

According to clean architecture, the architecture of Modian is designed like below:

![Modian Architecture V0.1](./docs/Modian%20Architecture%20V0.1.png)

---

## TODO

### C++ Practice
- [x] Use `#pragma once` instead of `#ifndef` in header file

### Architecture
- [ ] Introduce `application` layer to control status of input_engine and dictionary

### Thread Safety
- [ ] Implement Thread Safety Mechanisms: Introduce appropriate synchronization techniques to manage concurrent access to shared resources, preventing race conditions and ensuring data integrity.
- [ ] Designate Separate Read and Write Threads: Establish distinct threads for reading and writing operations to optimize performance and maintain data consistency during dictionary updates.
- [ ] Integrate SQLite for Dictionary Management: Transition to using SQLite for handling the dictionary, facilitating efficient storage, retrieval, and updates in a thread-safe manner.
- [ ] Develop a Strategy for Dynamic Dictionary Updates: Create a robust plan to handle real-time dictionary updates without disrupting ongoing input processing, ensuring that new entries are seamlessly incorporated.
- [ ] Conduct Comprehensive Testing: Perform thorough testing under various scenarios to identify and resolve potential threading issues, ensuring the stability and reliability of the input method.
