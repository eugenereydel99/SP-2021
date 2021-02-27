#!/bin/bash

zip_path_text="Specify the directory where the file is located, that you want to zip (for example, Documents/SP/lab1): "
file_path_text="Specify the directory where the file is located, that you want to unzip (for example, Documents/work): "

unzip_path_text="Specify unpacking place (for example, Documents/SP): "

want_to_continue()
{
  echo "Do you want to continue? [Y/n] : "
  read choice
  case "$choice" in
    [yY])
      return 0
      ;;
    [nN])
      return 1
      ;;
    *)
      return 2
  esac
}


menu(){

  echo "Select an action: 1 - archive, 2 - unpack, 3 - exit from program."
  read cmd
  case $cmd in
  ("1")
     read -p "$zip_path_text" file_path
     if [ -e "/$file_path" ]; then
        read -p "Type new name for you archive: " zip_name
        read -p "Type filename you want to archive: " file_name
        if [ -e "/$file_path/$file_name" ]; then 
           cd "/$file_path"
           tar -cf "$zip_name" "$file_name"
           ls
           echo "Successful."
        else
           echo "There is no such file."
        fi
     else
        echo "There is no such path."
     fi;;
  ("2")
     read -p "$file_path_text" file_path
     if [ -e "/$file_path" ]; then
        read -p "Type name of existing archive: " zip_name
        if [ -f "/$file_path/$zip_name" ]; then
           read -p "$unzip_path_text" unzip_path
           cd "/$file_path"
           tar -xf "$zip_name" -C "/$unzip_path"
           ls "/$unzip_path"
	   echo "Successful."
        else
           echo "THere is no such archive."
        fi
     else
        echo "There is no such path."
     fi;;
  ("3")
     echo "Exiting from the program..."
     exit 0;;
  (*)
     echo "There is no such command.";;
  esac
}

# MAIN
while true
do

  menu
  want_to_continue

  last_command=$?
  if [ "$last_command" = "1" ]; then
    echo "Exiting from the program..."
    break
  elif [ "$last_command" = "2" ]; then
    echo "There is no such command. Exiting from the program..."
    break
  fi
  
done


