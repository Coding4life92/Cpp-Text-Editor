#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>

std::string getUserInput()
{
  std::string input;
  std::cout << "Enter text: ";
  std::getline(std::cin, input);
  return input;
}

std::string readFile(const std::string &filename)
{
  std::ifstream file(filename);
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return content;
}

void writeFile(const std::string &filename, const std::string &content)
{
  std::ofstream file(filename);
  file << content;
}

void showProgressBar(int progress, int total)
{
  const int barWidth = 70;
  float progressRatio = static_cast<float>(progress) / total;
  int barProgress = static_cast<int>(progressRatio * barWidth);

  std::cout << "\033[35m["; // Set color to magenta
  for (int i = 0; i < barWidth; ++i)
  {
    if (i < barProgress)
      std::cout << "=";
    else
      std::cout << " ";
  }
  std::cout << "]\033[0m " << static_cast<int>(progressRatio * 100.0) << "%\r"; // Reset color to default
  std::cout.flush();
}

void performTask()
{
  const int total = 100;
  for (int i = 0; i <= total; ++i)
  {
    showProgressBar(i, total);
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulate some work
  }
  std::cout << std::endl;
}

class Editor
{
public:
  Editor();
  void run();

private:
  std::string m_content;

  void displayContent();
  void insertText();
  void deleteText();
  void saveAndQuit(const std::string &filename);
};

Editor::Editor()
{
  std::cout << "\033[2J\033[1;1H"; // Clear screen

  // Initialize the editor with an empty content
  m_content = "";
}

void Editor::saveAndQuit(const std::string &filename)
{
  writeFile(filename, m_content);
  performTask();
  std::cout << "Content saved to " << filename << ".\n";
}

void Editor::run()
{
  char choice;
  bool validChoice = true;
  std::string filename;

  while (validChoice)
  {
    displayContent();
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    std::cin.ignore(); // Ignore the newline character

    switch (choice)
    {
    case '1':
      insertText();
      validChoice = true;
      break;
    case '2':
      deleteText();
      validChoice = true;
      break;
    case '3':
      std::cout << "Enter the filename to save: ";
      std::getline(std::cin, filename);
      saveAndQuit(filename);
      validChoice = false;
      break;
    default:
      std::cout << "Invalid choice. Please try again.\n";
    }
  }
}

void Editor::displayContent()
{
  // Display the welcome message
  std::cout << "\033[1;37m-------------------------------\n";
  std::cout << "|                             |\n";
  std::cout << "| \033[1;30mWelcome to the Text Editor \033[1;37m |\n";
  std::cout << "|                             |\n";
  std::cout << "-------------------------------\n";

  std::cout << "\033[1;37mContent:\n";
  std::cout << m_content << "\n";

  // Display the options menu
  std::cout << "\033[1;37m---------------------\n";
  std::cout << "| \033[1;30mOptions           \033[1;37m|\n";
  std::cout << "---------------------\n";
  std::cout << "| \033[1;35m1. Insert text    \033[1;37m|\n";
  std::cout << "| \033[1;35m2. Delete text    \033[1;37m|\n";
  std::cout << "| \033[1;35m3. Save and exit  \033[1;37m|\n";
  std::cout << "---------------------\n";
}

void Editor::insertText()
{
  std::string text = getUserInput();
  m_content += text;
}

void Editor::deleteText()
{
  std::string text = getUserInput();
  size_t pos = m_content.find(text);
  if (pos != std::string::npos)
  {
    m_content.erase(pos, text.length());
  }
  else
  {
    std::cout << "Text not found.\n";
  }
}

int main()
{
  Editor editor;
  editor.run();

  return 0;
}