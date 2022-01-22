#include "Alg.h"

namespace alg
{

	size_t LevenshteinDistance(const std::string& firstWord, const std::string& secondWord)
	{
		if (firstWord.empty())
		{
			return secondWord.size();
		}
		else if (secondWord.empty())
		{
			return firstWord.size();
		}


		std::vector<size_t> prevRow(firstWord.size());
		std::vector<size_t> currentRow(firstWord.size());



		if (firstWord[0] == secondWord[0])
		{
			currentRow[0] = 0;
		}
		else
		{
			currentRow[0] = 1;
		}

		for (size_t iFirstWord = 1; iFirstWord < firstWord.size(); ++iFirstWord)
		{
			if (firstWord[iFirstWord] == secondWord[0])
			{
				currentRow[iFirstWord] = iFirstWord;
			}
			else
			{
				currentRow[iFirstWord] = std::min(iFirstWord, currentRow[iFirstWord - 1]) + 1;
			}
		}



		for (size_t iSecondWord = 1; iSecondWord < secondWord.size(); iSecondWord++)
		{
			std::swap(currentRow, prevRow);


			if (firstWord[0] == secondWord[iSecondWord])
			{
				currentRow[0] = iSecondWord;
			}
			else
			{
				currentRow[0] = std::min(iSecondWord, prevRow[0]) + 1;
			}

			for (size_t iFirstWord = 1; iFirstWord < firstWord.size(); ++iFirstWord)
			{
				if (firstWord[iFirstWord] == secondWord[iSecondWord])
				{
					currentRow[iFirstWord] = prevRow[iFirstWord - 1];
				}
				else
				{
					currentRow[iFirstWord] = std::min({ currentRow[iFirstWord - 1],prevRow[iFirstWord],prevRow[iFirstWord - 1] }) + 1;
				}
			}

		}

		return currentRow[firstWord.size() - 1];
	}
}