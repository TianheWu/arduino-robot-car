import serial
import tkinter as tk


init_x = 75
init_y = 100
path_exist = True
shortest_path = ''

ser = serial.Serial('COM9')
print("open serial", ser.name)         # check which port was really used

root_move = tk.Tk()
root_move.title("car move")
root_map = tk.Tk()
root_map.title("map")


def get_text():
    print('get text...')
    main_text = ''
    while True:
        data = ser.readline()
        data = data.strip()
        data = data.decode('utf-8', 'ignore')
        print('data:', data)
        if data == 'Shortest Path:':
            path = ser.readline()
            path = path.strip()
            path = path.decode('utf-8', 'ignore')
            global shortest_path
            shortest_path = path
            print('data:', path)
        if data == "MAP":
            count = 0
            print('get map text...')
            while count < 25:
                point_text = ser.readline()
                point_text = point_text.strip()
                point_text = point_text.decode('utf-8', 'ignore')
                main_text += point_text
                count += 1
            break
        if data == 'No path!':
            global path_exist
            path_exist = False
    print(main_text)
    return main_text


def transform_str2map():
    print('build map...')
    text_map = get_text()
    if path_exist == False:
        for i in range(len(text_map)):
            if text_map[i] == '.':
                text_map[i] = 'X'
    _map = [[0 for i in range(5)] for j in range(5)]
    _i = 0
    _j = 0
    count = 0
    for i in range(len(text_map)):
        if text_map[i] == 'X':
            _map[_i][_j] = 'X'
        elif text_map[i] == '.':
            _map[_i][_j] = '.'
        elif text_map[i] == 'E':
            _map[_i][_j] = 'E'
        elif text_map[i] == 'S':
            _map[_i][_j] = 'S'
        count += 1
        _j += 1
        if count % 5 == 0:
            _i += 1
            count = 0
            _j = 0
    print('map:')
    print(_map)
    return _map


def draw_rectangle(canvas, x, y, color):
    canvas.create_rectangle(x, y, x + 70, y + 70, fill=color, outline='black')


def generate_map(_root, _map):
    start_x = 0; start_y = 0
    canvas = tk.Canvas(_root, width=500, height=500, bg="white")
    canvas.create_text(250, 35, text='THE MAZE MAP ARDUINO SEND')
    canvas.create_text(350, 55, text='Shortest path: ' + shortest_path)
    canvas.create_text(350, 75, text='Created by Wu Tianhe')
    cur_x = init_x
    cur_y = init_y
    for i in range(len(_map)):
        for j in range(5):
            if _map[i][j] == 'X':
                draw_rectangle(canvas, cur_x, cur_y, 'black')
            elif _map[i][j] == '.':
                draw_rectangle(canvas, cur_x, cur_y, 'white')
            elif _map[i][j] == 'E':
                draw_rectangle(canvas, cur_x, cur_y, 'green')
                canvas.create_text(cur_x + 35, cur_y + 35, text='E', font=("Purisa", 12))
            elif _map[i][j] == 'S':
                draw_rectangle(canvas, cur_x, cur_y, 'yellow')
                canvas.create_text(cur_x + 35, cur_y + 35, text='S', font=("Purisa", 12))
                start_x = cur_x + 35; start_y = cur_y + 35
            cur_x += 70
        cur_y += 70
        cur_x = init_x
    line_x = start_x; line_y = start_y
    for i in shortest_path:
        if i == 'U':
            canvas.create_line(line_x, line_y, line_x, line_y - 70, width=2, fill='red')
            line_y -= 70
        elif i == 'D':
            canvas.create_line(line_x, line_y, line_x, line_y + 70, width=2, fill='red')
            line_y += 70
        elif i == 'L':
            canvas.create_line(line_x, line_y, line_x - 70, line_y, width=2, fill='red')
            line_x -= 70
        elif i == 'R':
            canvas.create_line(line_x, line_y, line_x + 70, line_y, width=2, fill='red')
            line_x += 70

    canvas.pack()


def key_up(e):
    ser.write(('U' + e.char.upper()).encode('ASCII'))
    print('up', e.char)


def key_down(e):
    ser.write(('D' + e.char.upper()).encode('ASCII'))
    print('down', e.char)
    if e.char == 'e':
        _map = transform_str2map()
        generate_map(root_map, _map)
        root_map.mainloop()


frame = tk.Frame(root_move, width=200, height=200)
frame.bind("<KeyPress>", key_down)
frame.bind("<KeyRelease>", key_up)
frame.focus_set()
# display
frame.pack()
root_move.mainloop()




