# 🛠️ Two-Pass Assembler — Systems Programming Lab

Welcome to the Two-Pass Assembler Project!  
This repository contains a lightweight assembler written in C, developed as part of the **Systems Programming Lab** course at **The Open University of Israel**.

---

## 🚀 Overview

This assembler translates a simplified assembly language into **14-bit machine code** using a two-pass approach. It supports macro expansion, symbol tables, and generates `.ob`, `.ent`, and `.ext` files.

---

## ✨ Features

✅ Two-pass parsing (symbol resolution + machine code generation)  
✅ Supports `.data`, `.string`, `.entry`, `.extern`, and macro definitions  
✅ Symbol table + label management  
✅ Outputs `.ob`, `.ent`, `.ext` files per input  
✅ Clear error messages with source line tracking  

---

## 📁 Project Structure

```
two-pass-assembler/
├── src/                # Core source and header files
├── test_inputs/        # Sample `.as` input files
├── test_output/        # Output files (.ob, .ent, .ext, .am)
├── makefile            # Build automation
├── README.md           # Project documentation
```

---

## ⚙️ How to Build & Run

### 🔧 Build

```bash
make
```

This compiles the assembler into an executable named `assembler`.

### ▶️ Run

```bash
./assembler test_inputs/input1.as
```

Generated output will be placed in the `test_output/` folder:
- `input1.ob` – Machine code
- `input1.ent` – Entries table
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

Each instruction is encoded into a 14-bit word using this structure:

| Bits     | Field                  | Description                                 |
|----------|------------------------|---------------------------------------------|
| 13–10    | Opcode (4 bits)        | Operation (`mov`, `add`, etc.)              |
| 9–7      | Source Addressing (3)  | Register/addressing mode of source operand  |
| 6–4      | Dest Addressing (3)    | Register/addressing mode of destination     |
| 1–0      | ARE (2 bits)           | A = Absolute, R = Relocatable, E = External |

#### 🧾 Example Encoding

Instruction: `mov R3, R1`  
→ Opcode: `0000`, Src: `011`, Dest: `001`, ARE: `00`  
**Result:** `00000110010000`

---

## 👨‍💻 Authors

👤 **Amir Kot** ([AmirKot](https://github.com/AmirKot)  
📘 B.Sc. Computer Science, The Open University of Israel  
🧑‍🤝‍🧑 Collaborated with [GitShiraz11](https://github.com/GitShiraz11)  

---

## 📄 License

This project is released under the [MIT License](LICENSE).  
Free to use, learn from, and improve.

---

## 🙌 Thank You

Thanks for checking out the project!  
Feel free to fork, star ⭐ the repo, or open an issue if you'd like to contribute or ask questions.

---

## 🔗 Connect

- 🔗 [LinkedIn](https://www.linkedin.com/in/amir-kot-0a7598369)
