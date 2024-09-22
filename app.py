from src.benchmark import benchmark
from src.colorgraph import color_graph


menu_map = {
    "0": benchmark,
    "1": color_graph,
}

if __name__ == "__main__":
    print("Here's the approximate graph coloring algorithms. Select what your function:\n\n"\
            "\t(n) - function\n"\
            "\t0 - benchmark\n"\
            "\t1 - color_graph\n")
    selected_function = input("Select function (n): ")
    menu_map[selected_function]()
    print("program finished")