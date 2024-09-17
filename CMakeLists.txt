cmake_minimum_required(VERSION 3.5)
project(dessinGrapheCmake VERSION 0.1.0)

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()
set(CMAKE_CXX_STANDARD 17) # Impossible d'utiliser plus récent avec OGDF
add_compile_options(-fpermissive) 

set(CMAKE_VERBOSE_MAKEFILE ON)

set(OS_TYPE ${CMAKE_SYSTEM_NAME})
#variable pour ogdf path
set(BUILD_TYPE "Release")

#chemin vers les librairies
set(OGDF_DIR "${CMAKE_SOURCE_DIR}/../Librairies/${OS_TYPE}/OGDF_${OS_TYPE}")
set(NLOHMANN_JSON_DIR "${CMAKE_SOURCE_DIR}/../Librairies/${OS_TYPE}/json-develop_${OS_TYPE}")
set(GLFW3_DIR "${CMAKE_SOURCE_DIR}/../Librairies/${OS_TYPE}/glfw3_${OS_TYPE}")
set(FREETYPE_DIR "${CMAKE_SOURCE_DIR}/../Librairies/${OS_TYPE}/freetype_${OS_TYPE}")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/../Librairies/${OS_TYPE}/glm_${OS_TYPE}")
set(GLEW_DIR "${CMAKE_SOURCE_DIR}/../Librairies/${OS_TYPE}/glew_${OS_TYPE}")


find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
if(${OS_TYPE} MATCHES "Linux")
    message("${OS_TYPE} OS")
    find_package(${GLFW3_DIR}/lib/cmake/glfw3/glfw3 REQUIRED)
    find_package(${OGDF_DIR}/lib/x86_64-linux-gnu/cmake/OGDF/ogdf REQUIRED)
    find_package(${NLOHMANN_JSON_DIR}/share/cmake/nlohmann_json/nlohmann_json REQUIRED)

    link_directories("${FREETYPE_DIR}/lib")
    link_directories("${GLEW_DIR}/lib64")

elseif(${OS_TYPE} MATCHES "Windows")
    message("${OS_TYPE} OS")
    add_library(glfw STATIC IMPORTED)
    add_library(OGDF STATIC IMPORTED)
    add_library(COIN STATIC IMPORTED)
    add_library(FREETYPE STATIC IMPORTED)
    add_library(glm STATIC IMPORTED)
    add_library(glew STATIC IMPORTED)

    set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${GLFW3_DIR}/build/src/Debug/glfw3.lib")
    set_target_properties(OGDF PROPERTIES IMPORTED_LOCATION "${OGDF_DIR}/build/${BUILD_TYPE}/OGDF.lib")
    set_target_properties(COIN PROPERTIES IMPORTED_LOCATION "${OGDF_DIR}/build/${BUILD_TYPE}/COIN.lib")
    set_target_properties(FREETYPE PROPERTIES IMPORTED_LOCATION "${FREETYPE_DIR}/build/Debug/freetyped.lib")
    set_target_properties(glm PROPERTIES IMPORTED_LOCATION "${GLM_DIR}/build/glm/${BUILD_TYPE}/glm.lib")
    set_target_properties(glew PROPERTIES IMPORTED_LOCATION "${GLEW_DIR}/lib/${BUILD_TYPE}/x64/glew32s.lib")
endif()

# Fichiers a compiler
add_executable(dessinGrapheCmake
    algos_graphe.cpp
    arete.cpp
    cellule.cpp
    display_openGL.cpp
    emplacement.cpp
    geometrie.cpp
    glouton_meilleur_deplacement.cpp
    graphe.cpp
    graphe_template.cpp
    grille.cpp
    intersection.cpp
    main.cpp
    manipulateur_json.cpp
    manipulateur_ogdf.cpp
    noeud.cpp
    placement_aleatoire.cpp
    placement_fmme.cpp
    placement_plus_proche.cpp
    placement_noeud_par_noeud.cpp
    placement_min_dist_max.cpp
    placement_ordonne.cpp
    position.cpp
    recuit_simule.cpp
    recuit_simule_depart_dynamique.cpp
    recuit_simule_dynamique.cpp
    selecteur_emplacement_aleatoire.cpp
    selecteur_emplacement_binaire.cpp
    selecteur_emplacement_multiple.cpp
    selecteur_noeud_aleatoire.cpp
    selecteur_noeud_binaire.cpp
    selecteur_noeud_multiple.cpp
    statistiques.cpp
    statistiques_template.cpp
    utilitaire.cpp
    utilitaire_template.cpp

    activation_function_layer.cpp
    edge_mcts.cpp
    fully_connected_layer.cpp
    game.cpp
    game_state.cpp
    layer.cpp
    matrix.cpp
    mcts.cpp
    network_alpha_go_zero.cpp
    neural_network.cpp
    neurone_layer.cpp
    softmax_layer.cpp
    softmax_layer_alpha_go_zero.cpp
    trainer.cpp
    training_data_generator.cpp
    tree_mcts.cpp

    tests_graphe.cpp
    textRendering.cpp
)

# Inclusion des fichiers header necessaires
include_directories(
    "${NLOHMANN_JSON_DIR}/include"  
    "${OGDF_DIR}/include"
    "${GLFW3_DIR}/include"
    "${GLEW_DIR}/include/GL"
    "${CMAKE_SOURCE_DIR}"
)

if(${OS_TYPE} MATCHES "Linux")
include_directories(
    "${FREETYPE_DIR}/include/freetype2"
    "${GLM_DIR}/include"
)
target_link_libraries(dessinGrapheCmake 
    "${FREETYPE_DIR}/lib/libfreetype.so"
    "${GLEW_DIR}/lib64/libGLEW.a"
)
elseif(${OS_TYPE} MATCHES "Windows")
include_directories(
    "${FREETYPE_DIR}/include"
    "${GLM_DIR}"
)
target_link_libraries(dessinGrapheCmake 
    FREETYPE
    glm
    glew
)
endif()


# Inclusion des libraries necessaires
target_link_libraries(dessinGrapheCmake 
    ${CMAKE_DL_LIBS}
    OpenGL::GL
    OpenMP::OpenMP_CXX

    OGDF
    COIN      
    glfw
)

if (MSVC)
    #Disable annoying warnings
    target_compile_options(dessinGrapheCmake PRIVATE /wd4068) #Pragma inconnu 'mark'
    target_compile_options(dessinGrapheCmake PRIVATE /wd4267) #conversion de 'size_t' en 'int'
    target_compile_options(dessinGrapheCmake PRIVATE /wd4244) #conversion de 'double' en 'int'
endif()