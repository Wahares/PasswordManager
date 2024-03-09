# PasswordManager
   
Console based password manager made as a college project.   
It encrypts out passwords in a safe way and stores them in files.   
After logging into a file it covers all of the basic actions with data such as:   
-adding/deleting/modifying password   
-adding/deleting category   
-searching through records   
-sorting by one or two fields at the same time   
   
   
## Opening file
At the start of this app you can either create new file or pick one. After picking file you can try to log into it.
![opening file](https://github.com/Wahares/PasswordManager/assets/102261228/b9c4f9ac-e42f-4680-a6a6-50a2f050136b)
   
   
## Corrupted file detetion
Application automaticly finds that the file is corrupted and will cancel logging process.
![corrupted file](https://github.com/Wahares/PasswordManager/assets/102261228/3dfd8c01-b11f-42bf-a05f-1da3046e417d)
   
   
## Adding password
In the process of adding password you can choose either you want to give your own password or generate automaticly. In both cases system checks if password is strong enough. Apart from that you can choose name of the account, login, domain and previously created category.
![adding pass](https://github.com/Wahares/PasswordManager/assets/102261228/9c36bc4e-cc12-4aa4-b4e4-d8129764baa0)
   
   
## Modifying password
Of course you can modify any field in any existing password and changes are saved automaticly. 
![modify pass](https://github.com/Wahares/PasswordManager/assets/102261228/4dd6a10b-36ae-4bf0-983a-b4aaed1e0c69)
   
   
# Encryption
Manager encrypt files in two ways:  
-"static" encryption based on BASE16 system that allows to change some of the data without the password (changing timestamp when provided incorrect password)   
-proper encryption with password that covers all of the protected data   

