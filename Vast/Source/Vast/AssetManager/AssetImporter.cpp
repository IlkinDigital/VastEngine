#include "vastpch.h"
#include "AssetImporter.h"

#include "Utils/FileIO/FileIO.h"
#include "Serialization/AssetSerializer.h"

namespace Vast {

    Ref<Texture2DAsset> AssetImporter::ImportTexture(const Filepath& imagePath, const Filepath& toPath)
    {
        OPTICK_EVENT();

        if (FileIO::IsImage(imagePath))
        {
            auto ta = CreateRef<Texture2DAsset>(imagePath.filename().stem().string(), toPath, UUID());
            std::ifstream fs(imagePath, std::ios::binary);
            if (fs.is_open())
            {
                DArray<char> data(std::filesystem::file_size(imagePath), '\0');
                fs.read(data.data(), data.size());
                ta->SetFileData(data);

                AssetSerializer as(m_Project, ta);
                as.Serialize();

                return ta;
            }
        }

        return nullptr;
    }

    void AssetImporter::ExportTexture(const Ref<Texture2DAsset>& asset, const Filepath& toPath)
    {
        OPTICK_EVENT();

        std::ofstream fs(toPath, std::ios::binary);
        fs.write(asset->GetFileData().data(), asset->GetFileData().size());
    }

}