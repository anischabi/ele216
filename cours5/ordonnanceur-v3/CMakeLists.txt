cmake_minimum_required(VERSION 3.10.0)
project(scheduler VERSION 0.1.0 LANGUAGES C)

add_executable(scheduler main.c
                        tabdyn_generique.c                       
                         file_generique.c 
                         fileprio.c
                         processus.c
)
