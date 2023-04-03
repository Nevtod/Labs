 
# r = 0.25
# sum = 250 - 135 / (1 + r) - 55 / (1 + r) / r
# for i in range(3):
#     sum -= 55 * (1 / (1 + r)**(i + 2))
# print(sum)

Cd = 0.12 / (1 - 0.12)
Rd = 0.2

mm = (1 + Cd) / (Cd + Rd)
print(mm)
M1 = 250
M2 = M1 - mm * 0.4
print(M2)
    