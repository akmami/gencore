import numpy as np
from Bio.Phylo.TreeConstruction import DistanceTreeConstructor, DistanceMatrix
from Bio import Phylo
import argparse
import matplotlib.pyplot as plt
from ete3 import Tree, TreeStyle

import os
os.environ["QT_QPA_PLATFORM"] = "offscreen"


def remove_inner_labels(tree):
    for clade in tree.find_clades():
        if not clade.is_terminal():
            clade.name = None


def read_phy(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
    
    n_sequences = int(lines[0].strip())
    
    labels = []
    for i in range(1, n_sequences + 1):
        labels.append(lines[i][:10].strip())
      
    matrix = []
    for line in lines[1:]:
        matrix.append(list(map(float, line[10:].strip().split())))
    
    distance_matrix = np.array(matrix)

    lower_triangle = []
    for i in range(len(distance_matrix)):
        row = distance_matrix[i][:i + 1].tolist()
        lower_triangle.append(row)

    return labels, lower_triangle


def construct_upgma_tree(labels, distance_matrix):
    dm = DistanceMatrix(labels, distance_matrix)
    constructor = DistanceTreeConstructor()
    tree = constructor.upgma(dm)
    
    # remove inner labels
    remove_inner_labels(tree)
    
    return tree


def save_ete_tree(newick_str, output_file, circular=False):
    tree = Tree(newick_str, format=1, quoted_node_names=True)
    
    if circular:
        ts = TreeStyle()
        ts.show_leaf_name = True
        ts.mode = "c"
        ts.branch_vertical_margin = 5
        ts.show_scale = False
        tree.render(output_file + "_ete.png", w=1000, units="px", tree_style=ts)
    else:
        ts = TreeStyle()
        ts.show_leaf_name = True
        ts.branch_vertical_margin = 5
        ts.show_scale = False
        tree.render(output_file + "_ete.png", w=1000, units="px", tree_style=ts)

    
    print(f"Tree saved to {output_file}_ete.png")


def save_tree(tree, output_file):
    # save plot
    plt.figure(figsize=(10, 7))
    Phylo.draw(tree)
    plt.savefig(output_file + ".png")
    plt.close()
    print(f"Tree saved to {output_file}.png")

    # save newick
    with open(output_file + ".newick", "w") as f:
        Phylo.write(tree, f, "newick")
        print(f"Tree saved to {output_file}.newick")

    # save ete3 plot
    save_ete_tree(tree.format("newick"), output_file)



def main(input_file, output_file):
    labels, matrix = read_phy(input_file)
    tree = construct_upgma_tree(labels, matrix)
    save_tree(tree, output_file)
    


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Construct a phylogenetic tree using UPGMA and save it as an image and Newick file.")
    parser.add_argument("input_file", help="Path to the input file containing the distances (phy format)")
    parser.add_argument("output_file", help="Path to the output file (without extension)")
    args = parser.parse_args()

    main(args.input_file, args.output_file)
