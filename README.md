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

- **Save** backups (single files or entire folders)
- **Delete** backups
- **Restore** backups

### **Install**

To install Buk, follow these steps:

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

- **Initialize a backup repository:**
    ```sh
    buk init
    ```

- **Configure path of backup folder:**
    ```sh
    buk config <PATH>
    ```

- **Save a backup:**
    ```sh
    buk save <list of files/folders>
    ```

- **Delete a backup:**
    ```sh
    buk delete <list of files/folders>
    ```

- **Restore a backup:**
    ```sh
    buk restore <list of files/folders>
    ```

- **Show commands list**
	```sh
	buk help
	```

### **Uninstall**

To uninstall **Buk**, run the following command:

- **Run the uninstaller script:**
    ```sh
    make uninstall
    ```
