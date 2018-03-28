#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <libplatform/libplatform.h>
#include <iostream>
#include "SimpleScene.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	//QApplication window;
	try
	{
		// ���� �������� 800x600, ���������� OpenGL Core Profile
		platform::RenderWindowOptions opts = { 800, 600, true };
		platform::RenderWindow window(opts);
		window.setScene(std::make_unique<SimpleScene>());
		window.show();
		return app.exec();
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		QMessageBox::warning(
			nullptr,
			QLatin1String("Sample05"),
			QString::fromUtf8(ex.what()));
	}
	
}