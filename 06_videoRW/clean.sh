#!/bin/bash

echo -e "\e[31;1m\nCmake files have been cleaned\n\e[0m"
sudo rm -r cmake* *Cache* *Make[Ff]ile* *.o 2>/dev/null

