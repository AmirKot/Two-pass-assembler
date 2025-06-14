
# 🛠️ Assembler Project — Systems Programming Lab

Welcome to the Assembler Project!  
This repository contains a two-pass assembler written in C, created as part of a university systems programming course.

---

## 🚀 Overview

This assembler translates a simplified assembly language into 14-bit machine code. It handles labels, macros, and multiple addressing modes, and produces the expected object, entry, and external files.

---

## ✨ Features

✅ Two-pass parsing (symbol table + machine code generation)  
✅ Handles `.data`, `.string`, `.entry`, `.extern` directives  
✅ Symbol table and macro expansion supported  
✅ Outputs `.ob`, `.ent`, `.ext` files  
✅ Clear error messages with line tracking

---

## 🏗️ Project Structure

```
assembler-project/
├── src/                # C source and header files
├── test_inputs/        # Input `.as` files
├── test_output/        # Generated `.ob`, `.ent`, `.ext`, `.am`
├── makefile            # Compile automation
├── README.md           # Project documentation
```

---

## 🧪 How to Build & Run

### 🧰 Build

```bash
make
```

This will compile the assembler and create an executable called `assembler`.

### ▶️ Run

```bash
./assembler test_inputs/input1.as
```

Outputs will appear in the `test_output/` folder:
- `input1.ob` – Encoded object file
- `input1.ent` – Entry label table
- `input1.ext` – External references

---

## 📝 Example Input (Assembly)

```asm
MAIN:   mov     S1.1, W
        add     r2, STR
        stop
STR:    .string "abcdef"
LENGTH: .data   6,-9,15
K:      .data   22
.extern W
.entry  MAIN
.entry  K
```

---

## 🧠 Machine Code Format (14-bit Encoding)

Each machine instruction is encoded into a **14-bit word** as follows:

| Bits     | Field                  | Description                                 |
|----------|------------------------|---------------------------------------------|
| 13–10    | Opcode (4 bits)        | Operation type (`mov`, `add`, etc.)         |
| 9–7      | Source Addressing (3)  | Addressing mode or register of source       |
| 6–4      | Dest Addressing (3)    | Addressing mode or register of destination  |
| 1–0      | ARE (2 bits)           | A = Absolute, R = Relocatable, E = External |

### 📦 Example Encoding

Instruction: `mov R3, R1`  
- Opcode = `0000`  
- Src = `011` (R3)  
- Dest = `001` (R1)  
- ARE = `00` (Absolute)

**Binary:** `00000110010000`

---

## 👥 Authors

👤 **Amir Kot [AmirKot]**  
🧑‍🤝‍🧑 Collaborated with [GitShiraz11]  
📘 The Open University — Systems Programming Lab

---

## 🛡️ License

This project is intended for educational use only.

---

## 🙌 Thank You!

Thanks for checking out the assembler project.  
If you have feedback or want to contribute, feel free to fork or open an issue.

