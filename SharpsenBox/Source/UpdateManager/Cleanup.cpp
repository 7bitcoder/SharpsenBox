#include <QDir>
#include "Cleanup.hpp"
#include "IConfig.hpp"

namespace sb
{
	void Cleanup::Run()
	{
		try
		{
			auto &downloadDir = Component<IConfig>::Get().GetDownloadDir();
			QDir dir(downloadDir);
			for(QString dirFile: dir.entryList())
			{
				dir.remove(dirFile);
			}
		}
		catch (...)
		{
			Error("Error ocured while removing temporary files");
		}
	}
}