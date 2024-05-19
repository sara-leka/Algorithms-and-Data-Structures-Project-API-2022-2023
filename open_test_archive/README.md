# Test aperti per testare la correttezza e la performance


In open_i.txt si trova l'input da dare al main.

Dopo aver scaricato lo [zip](https://github.com/sara-leka/Algorithms-and-Data-Structures-Project-API-2022-2023/blob/main/open_test_archive/open_output_archive.zip), in open_i_output.txt si trova l'output atteso.



Per compilare il [sorgente](https://github.com/sara-leka/Algorithms-and-Data-Structures-Project-API-2022-2023/tree/main/API_project_source.c) da riga di comando:
```
gcc -Wall -Werror -std=gnu11 -O2 -lm -g3 API_project_source.c -o project -fsanitize=address
```

Successivamente per eseguire e avere traccia dell'execution time:
```
time ./project < archivio_test_aperti/open_i.txt > output_local_i.txt
```

Infine per controllare la correttezza dell'output:
```
diff open_i_output.txt output_local_i.txt
```



