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
    buk config <path>
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
	buk -h
	buk --help
	```

- **Show the version that was installed**
	```sh
	buk -v
	buk --version
	```

- **Show the man page**
	```sh
	man buk
	```

### **Uninstall**

To uninstall **buk**:

- **Run the uninstaller script:**
    ```sh
    make uninstall
    ```

### **Flow diagrams**

Below are the flow diagrams for each command. Click on a command to expand its diagram:

<details>
<summary>init</summary>

![init flow](doc/diagrams/init.svg)

</details>

</details>

<details>
<summary>config</summary>

![config flow](doc/diagrams/config.svg)

</details>

<details>
<summary>save</summary>

![save flow](doc/diagrams/save.svg)

</details>

<details>
<summary>delete</summary>

![delete flow](doc/diagrams/delete.svg)

</details>

<details>
<summary>restore</summary>

![restore flow](doc/diagrams/restore.svg)
