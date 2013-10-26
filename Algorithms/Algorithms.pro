QT += widgets core gui

QMAKE_CXXFLAGS += -w -std=c++11

SOURCES += \
    CODE/codeeditor.cpp \
    CODE/highlighter.cpp \
    CORE_CLASSES/animatedalgorithm.cpp \
    DATA_STRUCTURES/VisualTreeElement.cpp \
    DATA_STRUCTURES/visualtree.cpp \
    DATA_STRUCTURES/visualstack.cpp \
    DATA_STRUCTURES/visuallistelement.cpp \
    DATA_STRUCTURES/visuallist.cpp \
    DATA_STRUCTURES/visualgraph.cpp \
    DATA_STRUCTURES/visualarrayelement.cpp \
    DATA_STRUCTURES/visualarray.cpp \
    DATA_STRUCTURES/node.cpp \
    DATA_STRUCTURES/edge.cpp \
    GRAPH_ALGORITHMS/dijsktra.cpp \
    GRAPH_ALGORITHMS/dfs.cpp \
    GRAPH_ALGORITHMS/bfs.cpp \
    GRAPH_ALGORITHMS/astar.cpp \
    HELPER_CLASSES/settings.cpp \
    HELPER_CLASSES/cross.cpp \
    HELPER_CLASSES/arrow.cpp \
    SORT/shellsort.cpp \
    SORT/selectionsort.cpp \
    SORT/quicksort.cpp \
    SORT/insertionsort.cpp \
    SORT/bubblesort.cpp \
    TABS/texttabwidget.cpp \
    TABS/tabWidget.cpp \
    TABS/entry.cpp \
    TABS/codetabwidget.cpp \
    TABS/animationtabwidget.cpp \
    TREE_ALGORITHMS/preorder.cpp \
    TREE_ALGORITHMS/postorder.cpp \
    TREE_ALGORITHMS/inorder.cpp \
    mainwindow.cpp \
    main.cpp

HEADERS += \
    CODE/codeeditor.h \
    CODE/highlighter.h \
    CORE_CLASSES/animatedalgorithm.h \
    DATA_STRUCTURES/VisualTreeElement.h \
    DATA_STRUCTURES/visualtree.h \
    DATA_STRUCTURES/visualstack.h \
    DATA_STRUCTURES/visuallistelement.h \
    DATA_STRUCTURES/visuallist.h \
    DATA_STRUCTURES/visualgraph.h \
    DATA_STRUCTURES/visualarrayelement.h \
    DATA_STRUCTURES/visualarray.h \
    DATA_STRUCTURES/node.h \
    DATA_STRUCTURES/edge.h \
    GRAPH_ALGORITHMS/dijsktra.h \
    GRAPH_ALGORITHMS/dfs.h \
    GRAPH_ALGORITHMS/bfs.h \
    GRAPH_ALGORITHMS/astar.h \
    HELPER_CLASSES/settings.h \
    HELPER_CLASSES/cross.h \
    HELPER_CLASSES/arrow.h \
    SORT/sort.h \
    SORT/shellsort.h \
    SORT/selectionsort.h \
    SORT/quicksort.h \
    SORT/insertionsort.h \
    SORT/bubblesort.h \
    TABS/texttabwidget.h \
    TABS/tabWidget.h \
    TABS/entry.h \
    TABS/codetabwidget.h \
    TABS/animationtabwidget.h \
    TREE_ALGORITHMS/preorder.h \
    TREE_ALGORITHMS/postorder.h \
    TREE_ALGORITHMS/inorder.h \
    mainwindow.h

FORMS += \
    HELPER_CLASSES/settings.ui \
    TABS/tabWidget.ui \
    TABS/entry.ui \
    mainwindow.ui

RESOURCES += \
    resource.qrc
