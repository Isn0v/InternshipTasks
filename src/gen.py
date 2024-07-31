from random import randint, shuffle
import matplotlib.pyplot as plt


def plot_walls_and_treasure(walls, treasure):
    fig, ax = plt.subplots()
    for wall in walls:
        x1, y1, x2, y2 = wall
        ax.plot([x1, x2], [y1, y2], 'b-', lw=2)
    points = [
        (44.864757862142739, 49.009187422290267),
        (45.135242137857261, 50.990812577709733)
    ]

    # Отметить местоположение сокровища
    for point in points:
        ax.plot(point[0], point[1], 'ro', markersize=10)
    ax.plot(treasure[0], treasure[1], 'ro', markersize=10)

    # Установить границы графика и метки
    ax.set_xlim(-10, 110)
    ax.set_ylim(-10, 110)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_title('Walls and Treasure Location')
    ax.grid(True)

    plt.show()


if __name__ == '__main__':
    walls = [
        (5, 0, 25, 100),
        (0, 10, 100, 30),
        (10, 0, 100, 20),
        (20, 100, 40, 0),
        (15, 0, 15, 100),
        (0, 25, 100, 50),
        (55, 0, 55, 100),
        (0, 10, 90, 100),
        (0, 40, 100, 60),
        (60, 0, 60, 100),
        (0, 55, 100, 70),
        (0, 30, 60, 100),
        (0, 70, 100, 80),
        (70, 0, 70, 100),
        (40, 0, 40, 100),
        (0, 80, 100, 85),
        (80, 0, 80, 100),
        (45, 0, 45, 100),
        (0, 85, 100, 90),
        (85, 0, 85, 100),
        (0, 90, 100, 95),
        (90, 0, 90, 100),
        (55, 0, 55, 100),
        (0, 95, 100, 100),
        (95, 0, 95, 100),
        (60, 0, 60, 100),
        (0, 100, 100, 100),
        (65, 0, 65, 100),
        (70, 0, 70, 100),
    ]
    treasure = (50.5, 50.5)
    # treasure = (95.999999999994472, 49.999996672921107)
    # walls = [
    #     (20, 0, 37, 100),
    #     (40, 0, 76, 100),
    #     (85, 0, 0, 75),
    #     (100, 90, 0, 90),
    #     (0, 71, 100, 61),
    #     (0, 14, 100, 38),
    #     (100, 47, 47, 100),
    # ]
    # treasure = (50, 27)
    plot_walls_and_treasure(walls, treasure)

# boundary_limits = (top, right, bottom, left) limits of expansion


def gen_expansion(boundary_limits, capacity):
    points = sorted([randint(0, capacity) for _ in range(3)])
    expansion = [points[0], points[1] - points[0],
                 points[2] - points[1], capacity - points[2]]

    # Adjust expansion based on boundary_limits
    for i in range(4):
        if expansion[i] > boundary_limits[i]:
            excess = expansion[i] - boundary_limits[i]
            expansion[i] = boundary_limits[i]
            remaining_indices = [j for j in range(4) if j != i]
            shuffle(remaining_indices)
            for idx in remaining_indices:
                if excess <= 0:
                    break
                addition = min(excess, boundary_limits[idx] - expansion[idx])
                expansion[idx] += addition
                excess -= addition

    return expansion


def expand(id, point, field, expansion):
    x = point[0]
    y = point[1]
    field[y][x] = id
    d_xy = [(0, 1), (1, 0), (0, -1), (-1, 0)]
    for i in range(4):
        d_x, d_y = d_xy[i]
        for val in range(1, expansion[i] + 1):
            field[y + val * d_y][x + val * d_x] = id


def point_in_field(point, field):
    x = point[0]
    y = point[1]
    return x >= 0 and x < len(field[0]) and y >= 0 and y < len(field)


def get_boundary_limits(point, field):
    x = point[0]
    y = point[1]
    d_xy = [(0, 1), (1, 0), (0, -1), (-1, 0)]
    boundary_limits = [0, 0, 0, 0]
    for i in range(4):
        d_x, d_y = d_xy[i]
        j = 0
        while (True):
            j += 1
            if not point_in_field((x + j * d_x, y + j * d_y), field) or field[y + j * d_y][x + j * d_x] != 0:
                boundary_limits[i] = j - 1
                break
    return boundary_limits


def is_field_filled(field):
    for i in range(len(field)):
        for j in range(len(field[0])):
            if field[i][j] == 0:
                return False
    return True


def generate_pizza_test(height, width, pizzerias_counter):

    height = 8
    width = 8
    pizzerias_counter = 20
    total_capacity = width * height - pizzerias_counter

    all_expansions = []
    all_pizza_points = []
    all_capacities = []

    field = [[0 for _ in range(width)] for _ in range(height)]

    while (not is_field_filled(field)):
        field = [[0 for _ in range(width)] for _ in range(height)]
        total_capacity = width * height - pizzerias_counter
        all_expansions = []
        all_pizza_points = []
        all_capacities = []
        for i in range(1, pizzerias_counter + 1):
            x, y = randint(0, width - 1), randint(0, height - 1)
            while field[y][x] != 0:
                x, y = randint(0, width - 1), randint(0, height - 1)
            boundary_limits = get_boundary_limits((x, y), field)
            capacity = randint(0, min(sum(boundary_limits), total_capacity))
            total_capacity -= capacity
            expansion = gen_expansion(boundary_limits, capacity)
            expand(i, (x, y), field, expansion)

            all_expansions.append(expansion)
            all_pizza_points.append((x, y))
            all_capacities.append(capacity)

    return all_expansions, all_pizza_points, all_capacities

# for i in range(height - 1, -1, -1):
#     print(" ".join(map(str, field[i])))

# # print(*all_expansions, sep=" ")
# # print(*all_pizza_points, sep=" ")

# with open("test/data/Pizza/input_9.txt", "w") as f:
#     f.write(f"{width} {height} {pizzerias_counter}\n")
#     for i in range(1, pizzerias_counter + 1):
#         f.write(f"{all_pizza_points[i - 1][0] + 1} {all_pizza_points[i - 1][1] + 1} {all_capacities[i - 1]}\n")
#     f.write("0\n")
# with open("test/data/Pizza/expected_9.txt", "w") as f:
#     for a, b, c, d in all_expansions:
#         f.write(f"{a} {b} {c} {d}\n")
#     f.write("\n")

# for i in range(8, 12):
#     with open(f"test/data/TreasureHunt/expected_{i}.txt", "w") as f:
#         pass
#     with open(f"test/data/TreasureHunt/input_{i}.txt", "w") as f:
#         pass


def count_reversed_inversions_naive(array):
    count = 0
    for i in range(len(array)):
        for j in range(i + 1, len(array)):
            if array[i] > array[j]:
                count += 1
    return count


def generate_railroads_test(coaches_count):
    array_generated = [i + 1 for i in range(coaches_count)]
    shuffle(array_generated)
    answer = ""

    inv = count_reversed_inversions_naive(list(array_generated))
    if (inv % 2 == 0):
        answer = "YES"
    else:
        answer = "NO"
    return list(array_generated), answer


# if __name__ == "__main__":
#     n = 1000
#     arr, ans = generate_railroads_test(n)

#     with open(f"test/data/Railroads/input_8.txt", "w") as f:
#         f.write(f"{n}\n")
#         for i in arr:
#             f.write(f"{i} ")
#         f.write("0\n0\n")
#     with open(f"test/data/Railroads/expected_8.txt", "w") as f:
#         f.write(f"\n{ans}\n\n")
