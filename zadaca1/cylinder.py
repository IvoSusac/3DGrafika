import math

file = open("cylinder.obj", "w")

r = 1
vertexNum = 20

angle = 360 / vertexNum

#lv = []
br = 0

while (br != 360):
    circle1X = r * math.cos(br / 360 * 2 * math.pi)
    circle1Y = r * math.sin(br / 360 * 2 * math.pi)

    file.write("v " + str(circle1X) + " " + str(circle1Y) + " " + "-1" + "\n")
    #lv.append(str(circle1X) + " " + str(circle1Y) + " " + "-1")

    file.write("v " + str(circle1X) + " " + str(circle1Y) + " " + "-1" + "\n")
    #lv.append(str(circle1X) + " " + str(circle1Y) + " " + "-1")

    br = br + angle

br = 0

while (br != 360):
    circle2X = r * math.cos(br / 360 * 2 * math.pi)
    circle2Y = r * math.sin(br / 360 * 2 * math.pi)

    file.write("v " + str(circle2X) + " " + str(circle2Y) + " " + "1" + "\n")
    #lv.append(str(circle2X) + " " + str(circle2Y) + " " + "1")

    file.write("v " + str(circle2X) + " " + str(circle2Y) + " " + "1" + "\n")
    #lv.append(str(circle2X) + " " + str(circle2Y) + " " + "-1")

    br = br + angle


#ln = []

br2 = 0

while (br2 != 360):
    normX = math.cos(br2 / 360 * 2 * math.pi)
    normY = math.sin(br2 / 360 * 2 * math.pi)

    file.write("vn " + str(normX) + " " + str(normY) + " " + "0\n")
    #ln.append(str(normX) + " " + str(normY) + " " + "0")
    file.write("vn 0 0 1\n")
    #ln.append("0 0 1")

    br2 = br2 + angle

lvar = [1, 3, vertexNum * 2 + 1, 3, vertexNum * 2 + 3, vertexNum * 2 + 1, 1]

while (lvar[6] != vertexNum * 2 + 1):
    if (lvar[6] == vertexNum * 2 - 1):
        if (lvar[0] == vertexNum * 2 - 1):
            lvar[6] = 1
        if (lvar[3] == vertexNum * 2 + 1):
            lvar[3] = vertexNum * 4 - 1
        if (lvar[4] == vertexNum * 4 + 1):
            lvar[4] = vertexNum * 2 - 1
        if (lvar[5] == vertexNum * 4 - 1):
            lvar[5] = 1
    
    file.write("f " + str(lvar[0]) + "//" + str(lvar[6]) + " " + str(lvar[1]) + "//" + str(lvar[6]) + " " + str(lvar[2]) + "//" + str(lvar[6]) + "\n")

    file.write("f " + str(lvar[3]) + "//" + str(lvar[6]) + " " + str(lvar[4]) + "//" + str(lvar[6]) + " " + str(lvar[5]) + "//" + str(lvar[6]) + "\n")

    for i in range(7):
        lvar[i] = lvar[i] + 2
    
brbaza1 = 2
temp1 = 2
temp2 = 2
temp3 = 4

while (brbaza1 != vertexNum * 2):
    file.write("f " + str(temp1) + "//" + str(brbaza1) + " " + str(temp3) + "//" + str(brbaza1) + " " + str(temp2) + "//" + str(brbaza1) + "\n")

    temp2 = temp2 + 2
    temp3 = temp3 + 2
    brbaza1 = brbaza1 + 2

brbaza2 = 2
temp1 = vertexNum * 2 + 2
temp2 = vertexNum * 2 + 4
temp3 = vertexNum * 2 + 2

while (brbaza2 != vertexNum * 2):
    file.write("f " + str(temp1) + "//" + str(brbaza2) + " " + str(temp3) + "//" + str(brbaza2) + " " + str(temp2) + "//" + str(brbaza2) + "\n")

    temp2 = temp2 + 2
    temp3 = temp3 + 2
    brbaza2 = brbaza2 + 2


file.close()
    

