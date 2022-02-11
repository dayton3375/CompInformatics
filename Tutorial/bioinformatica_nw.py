import numpy as np

s1 = "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT"
s2 = "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT"

# Create Matrices
main_matrix = np.zeros((len(s1) + 1, len(s2) + 1))
match_checker_matrix = np.zeros((len(s1), len(s2)))

# Providing the scores for the match, mismatch and gap
match = 1
mismatch = -1
gap = -2

# Fill the match checker matrix according to match or mismatch
for i in range (len(s1)):
    for j in range(len(s2)):
        if s1[i] == s2[j]:
            match_checker_matrix[i][j] = match
        else:
            match_checker_matrix[i][j] = mismatch

# print(match_checker_matrix)

# Filling up the matrix using Needleman_Wunsh algorithm
# Initialization
for i in range(len(s1) + 1):
    main_matrix[i][0] = i * gap
for j in range (len(s2) + 1):
    main_matrix[0][j] = j * gap

# Matrix Fill
for i in range(1, len(s1) + 1):
    for j in range(1, len(s2) + 1):
        main_matrix[i][j] = max(main_matrix[i - 1][j - 1] + match_checker_matrix[i - 1][j - 1], main_matrix[i - 1][j] + gap, main_matrix[i][j - 1] + gap)

# print(main_matrix)

# Traceback
aligned_1 = ""
aligned_2 = ""

ti = len(s1)
tj = len(s2)

while (ti > 0 and tj > 0):
    if (ti > 0 and tj > 0 and main_matrix[ti][tj] == main_matrix[ti - 1][tj - 1] + match_checker_matrix[ti - 1][tj - 1]):
        aligned_1 = s1[ti - 1] + aligned_1
        aligned_2 = s2[tj - 1] + aligned_2

        ti -= 1
        tj -= 1

    elif (ti > 0 and main_matrix[ti][tj] == main_matrix[ti - 1][tj] + gap):
        aligned_1 = s1[ti - 1] + aligned_1
        aligned_2 = "-" + aligned_2

        ti -= 1

    else:
        aligned_1 = "-" + aligned_1
        aligned_2 = s2[tj-1] + aligned_2

        tj -= 1

# test
print(aligned_1)
print(aligned_2)