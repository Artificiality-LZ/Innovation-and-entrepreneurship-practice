# SM3_Rho

SM3的Rho攻击简化版

使用两个map来记录产生的结果

SM3_IV_BOOL表示部分IV，用于检测IV的部分bit是否相同

![_18085(NOJ9(6VAB~`$~WB2](https://user-images.githubusercontent.com/105494706/181440735-99bbffc4-686b-430b-aa51-1c82018a17d5.png)

这里在SM3_IV_BOOL中重载了<运算符，用于map的排列

![IXA{9Y)J@FPTY%OGA6QS{XY](https://user-images.githubusercontent.com/105494706/181441067-b9363d25-0b34-4ac5-9b04-ee9ee4732cd1.png)

攻击的长度由常量bool_iv_length控制，当值为1时表示32bit，为2时表示64bit


随机初始化一个B然后循环查找是否有重复，重复则输出结果

运行结果：

![8_F6{0G4AU4EJVOV02EA21P](https://user-images.githubusercontent.com/105494706/181663783-77afb015-fba7-4b7f-98af-0cbc7bf8a9fb.png)


未组队，独立完成所有内容


