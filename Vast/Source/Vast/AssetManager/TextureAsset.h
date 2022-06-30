#pragma once

#include "Asset.h"

namespace Vast {

	class TextureAsset : public Asset
	{
	public:
		TextureAsset(const String& name, const Filepath& path, UUID uuid);

		const DArray<char>& GetFileData() const { return m_FileData; }
		void SetFileData(const DArray<char>& fileData) { m_FileData = fileData; }
	private:
		DArray<char> m_FileData; // Holds data of an imported file
	};

}