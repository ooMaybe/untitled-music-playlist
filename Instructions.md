# ENSC 151 Project Guide - Using GitHub Desktop 

## 📋 Branch Strategy Overview

- **`main`** - Production-ready code (don't touch!)
- **`dev`** - Integration branch where all features come together
- **`Norbu`, `Alex`** - Individual feature branches (one per team member)

### 💡 The Golden Rules:
1. ✅ Work ONLY on your own branch
2. ✅ Pull latest changes from `dev` before starting work
3. ✅ Merge to `dev` when your feature is complete
4. ❌ Never work directly on `dev` or `main`
5. Write clear descriptions like "Fix bug in calculator function" not "stuff"
6. Pull from `dev` daily - Even if you're not done, stay up to date
7. Make sure your code works before merging to `dev` unless otherwise discussed
8. Communicate & let your team know when you merge to `dev`
9. Commit after each small feature/fix, not at the end of the day

---

## 🚀 Initial Setup (Do This Once)

### 1. Clone the Repository
1. Open **GitHub Desktop**
2. Click **File** → **Clone Repository**
3. Select your project repository
4. Choose a local path and click **Clone**

### 2. Create Your Personal Branch
1. Make sure you're on the `dev` branch (check top bar)
2. Click **Current Branch** dropdown at the top
3. Click **New Branch**
4. Name it with your name (e.g., `Derek`)
5. Make sure "Create branch based on: **dev**" is selected
6. Click **Create Branch**

### 3. Publish Your Branch
1. After creating your branch, click **Publish branch**
2. This uploads your branch to GitHub so others can see it

---

## 💻 Daily Workflow

### Step 1: Get Latest Changes from `dev`

**Do this EVERY TIME before you start coding!**

1. Click **Current Branch** dropdown
2. Select **`dev`** branch
3. Click **Fetch origin** (top right)
4. If changes exist, click **Pull origin**
5. Switch back to **your branch** (e.g., `Krishang`)
6. Click **Current Branch** → **Choose a branch to merge into Bob**
7. Select **`dev`**
8. Click **Create a merge commit**
9. If there are conflicts, resolve them (see Conflict Resolution section)

---

### Step 2: Work on Your Code

1. Make sure you're on **your branch** (check top bar)
2. Open your code editor and make changes
3. Save your files

---

### Step 3: Commit Your Changes

1. Go to **GitHub Desktop**
2. You'll see your changed files in the left sidebar
3. Check the boxes next to files you want to commit
4. Write a **commit message** (bottom left):
   - **Summary**: Brief description (e.g., "Add login function")
   - **Description** (optional): More details
5. Click **Commit to [your-branch-name]**

**Commit Often!** Small, frequent commits are better than one huge commit.

---

### Step 4: Push Your Changes

1. After committing, click **Push origin** (top right)
2. This uploads your commits to GitHub

---

### Step 5: Merge Your Feature into `dev` (When Complete)

**Only do this when your feature is DONE and TESTED!**

1. Make sure all your changes are committed and pushed
2. Click **Current Branch** dropdown
3. Click **Create Pull Request**
4. This opens GitHub in your browser
5. Set:
   - **Base**: `dev`
   - **Compare**: `your-branch` (e.g., `Karam`)
6. Add a title and description
7. Click **Create Pull Request**
8. Ask a teammate to review (optional but recommended)
9. Click **Merge Pull Request** → **Confirm Merge**
10. Delete your branch if you're done (optional)

---

## 🔥 Handling Merge Conflicts

Conflicts happen when you and a teammate edit the same lines of code.

### When You See a Conflict:

1. GitHub Desktop will show "⚠️ Resolve conflicts"
2. Please do NOT proceed with the merge yet. Contact Karam for help resolving conflicts. 