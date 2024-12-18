#!/bin/bash

echo "Deploy script started"
scp /home/felisasi/DO6_CICD-2/builds/dSnnjSy6/0/students/DO6_CICD.ID_356283/felisasi_student.21_school.ru/DO6_CICD-2/src/cat/s21_cat root@10.10.0.2:/usr/local/bin/
echo "file cat copied"
scp /home/felisasi/DO6_CICD-2/builds/dSnnjSy6/0/students/DO6_CICD.ID_356283/felisasi_student.21_school.ru/DO6_CICD-2/src/grep/s21_grep root@10.10.0.2:/usr/local/bin/
echo "file grep copied"
echo "Deploy script done"