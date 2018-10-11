# 使用Qt Designer构建AWTK的界面

## 一、方法

* 1.用QT新建一个Application项目.
* 2.打开窗口界面编辑器，设置窗口的大小为需要的大小，删除centralWidget之外的其它控件。
* 3.在窗口中添加需要的控件。
* 4.保存文件。
* 5.在命令行下，进入awtk目录，用qt\_to\_xml将Qt的UI文件转换成AWTK的XML文件：

```
./bin/qt_to_xml /Users/jim/qt-app/demo1/mainwindow.ui mainwindow.xml
```
> 不同平台会有一些差异，请自行调整。

* 6.手工编辑生成的XML，根据需要做些调整。

## 二、支持的控件

| QT控件   |     AWTK控件      |  说明 |
|----------|:-------------:|------:|
| QDialog | dialog | 
| QMainWindow | window | 
| QLabel | label | 
| QLineEdit | edit | 
| QWidget | view | 
| QGraphicsView | image | 
| QComboBox | combo_box | 
| QPushButton | button | 
| QCheckBox | check_button | 
| QRadioButton | radio_button | 
| QProgressBar | progress_bar | 暂时不支持垂直progress_bar | 
| QSpinBox | spin_box | 
| QDoubleSpinBox | spin_box | 
| QSlider | slider | 暂时不支持垂直slider |

## 三、限制

目前主要用于辅助确定控件坐标，style等属性不会自动转换。


