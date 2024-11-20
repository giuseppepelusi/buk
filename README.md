```
 _           _
| |         | |
| |__  _   _| | __
| '_ \| | | | |/ /
| |_) | |_| |   <
|_.__/ \__,_|_|\_\
```

## **A simple command-line tool for backups**

Similar to Git but focused on backups rather than versioning, and easily integrable into scripts

### **Features**

- **Save** backups (single files or entire directories)
- **Delete** backups
- **Restore** backups

### **Install**

To install **buk**, follow these steps:

1. **Clone the repository:**
   ```sh
   git clone https://github.com/giuseppepelusi/buk.git
   ```

   or

   ```sh
   git clone git@github.com:giuseppepelusi/buk.git
   ```

2. **Move to the repository directory:**
    ```sh
    cd buk
    ```

3. **Run the installer script:**
    ```sh
    make install
    ```

### **Usage**

After installing, you can use the **buk** command as follows:

- **Initialize the current repository:**
    ```sh
    buk init
    ```

- **Configure the backup directory path (default: /home/username/.buk_backups):**
    ```sh
    buk config <PATH>
    ```

- **Save a backup:**
    ```sh
    buk save <file|dir> [<file|dir>...]
    ```

- **Delete a backup:**
    ```sh
    buk delete <file|dir> [<file|dir>...]
    ```

- **Restore a backup:**
    ```sh
    buk restore <file|dir> [<file|dir>...]
    ```

- **Show the list of available commands**
	```sh
	buk help
	```

### **Uninstall**

To uninstall **buk**:

- **Run the uninstaller script:**
    ```sh
    make uninstall
    ```
