#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab12.13(1)/lab12.13(1).cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
        TEST_METHOD(TestDeleteNode)
        {
            Tree trie;
            Note note1 = { "Doe", "John", "123456789", {1, 4, 1990} };
            Note note2 = { "Smith", "Alice", "987654321", {15, 8, 1985} };

            trie.addNote(note1);
            trie.addNote(note2);

            trie.deleteNote("Doe", "John");
            vector<Note> result;
            bool foundDoe = trie.findNoteByLastName("Doe", result);

            Assert::IsFalse(foundDoe);
        }
	};
}