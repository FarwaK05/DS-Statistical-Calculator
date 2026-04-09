# 📊 StatPro — Advanced Statistical & Probability Calculator

A full-stack statistical calculator with a **C++ backend server** and a **React.js frontend**. StatPro lets you analyze datasets, compute probabilities, and solve independent event problems — all through a sleek dark-themed browser UI.

---

## 🖥️ Preview

> **StatPro** — Advanced Statistical & Probability Calculator

The app is divided into three main sections:
- **Dataset & Distribution** — Add values and visualize a bar chart in real time
- **Basic Probability** — Compute combinations, permutations, and binomial probability
- **Independent Event Solver** — Calculate all set-theory probability operations between two events

---

## ✨ Features

### 📈 Dataset & Distribution
- Add values to a live dataset
- Auto-renders a **bar chart** showing frequency distribution
- Displays the dataset sequence visually
- One-click calculations:
  - **Mean**
  - **Median**
  - **Mode**
  - **Standard Deviation**
- **CLR ALL** button to reset the dataset

### 🎲 Basic Probability
- Inputs: `n` (Trials), `r/k` (Success), `p` (Probability, 0.0–1.0)
- Calculates:
  - **nCr** — Combinations
  - **nPr** — Permutations
  - **Binomial** — Binomial probability P(X = k)

### 🔀 Independent Event Solver
- Inputs: `P(A)` and `P(B)`
- Instantly computes:
  - **P(A') NOT A**
  - **P(B') NOT B**
  - **P(A∩B) INTERSECT**
  - **P(A∪B) UNION**
  - **P(AΔB) XOR**
  - **P((A∪B)') NEITHER**
- Each field has its own **CALC** button and a **+** (use result) shortcut
- **RESET ALL FIELDS** to clear all inputs

### 🕓 History
- Automatically logs every calculation result
- Shows label + value (e.g. `MEAN 4.3750`, `P(ANB) 0.2000`)
- **+ button** on each entry to reuse results directly
- **UNDO / REDO** support for history navigation

### ⚡ Last Answer (ANS)
- Displays the **last computed result** in the top-right corner
- **USE ANS** button to carry the result into the next calculation

---

## 🛠️ Tech Stack

| Layer | Technology |
|-------|------------|
| Backend | C++ (`main.cpp`, `httplib.h`) |
| Frontend | React.js |
| Styling | CSS (`App.css`) |
| Data Storage | JSON (`dataset.json`, `history.json`) |
| Data Structures | Binary Search Tree (`BST.h`) |
| Communication | HTTP (C++ server ↔ React client) |

---

## 🚀 How to Run

> ⚠️ Run **both** the backend and frontend for the app to work.

### Step 1 — Start the C++ Backend Server

```bash
cd StatCalc
./server.exe
```

> On Windows, you can also double-click `server.exe` directly.

The server starts and listens for API requests (default: `http://localhost:8080` — verify port in `main.cpp`).

To **recompile** from source:

```bash
g++ main.cpp -o server -std=c++17
./server
```

---

### Step 2 — Start the React Frontend

Open a **new terminal**:

```bash
cd stat-ui
npm install       # First time only
npm start
```

Opens automatically at **`http://localhost:3000`**.

---

### ✅ Usage

1. Make sure `server.exe` is running in the background.
2. Open your browser at `http://localhost:3000`.
3. Add dataset values and click **Mean / Median / Mode / STD DEV** to compute.
4. Use the **Basic Probability** panel for nCr, nPr, and Binomial calculations.
5. Enter `P(A)` and `P(B)` in the **Independent Event Solver** and click CALC on any field.
6. View and reuse past results from the **History** panel.
7. Use **USE ANS** to chain results across calculations.

---

## 🧰 Prerequisites

| Tool | Purpose |
|------|---------|
| Windows OS | Required to run `server.exe` directly |
| G++ / MinGW | To recompile the C++ backend from source |
| Node.js & npm | To run the React frontend |

---

## 🖼️ Screenshot

<img width="964" height="852" alt="image" src="https://github.com/user-attachments/assets/ada43a4e-c343-4ca7-b897-c240cc201a01" />
<img width="987" height="848" alt="image" src="https://github.com/user-attachments/assets/9623440d-b3c5-4888-84dd-822a02962d44" />
<img width="956" height="642" alt="image" src="https://github.com/user-attachments/assets/970c0a59-40f6-4856-93e4-2183cbcf1b89" />

---

## 👩‍💻 Author

**Farwa K** — [@FarwaK05](https://github.com/FarwaK05)

---

## 📄 License

This project is for educational purposes.
