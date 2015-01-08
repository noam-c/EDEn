#include "DialogueEntry.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_DIA_CONTR;

DialogueEntry::DialogueEntry(DialogueEntryType type, const std::string& text, const std::shared_ptr<Task>& task) :
   task(task),
   type(type),
   text(text)
{
   int openIndex = 0;
   int closeIndex = 0;

   for(;;)
   {
      int nextOpenIndex = text.find('<', openIndex+1);
      int nextCloseIndex = text.find('>', closeIndex+1);

      if(nextOpenIndex < 0)
      {
         if(nextCloseIndex >= 0)
         {
            T_T("Extra '>' character detected in dialogue line."
                " Please balance your dialogue script brackets (< and >).");
         }

         break;
      }
      else if(nextCloseIndex < 0)
      {
         T_T("Found '<' without matching '>' in dialogue line."
            " Please balance your dialogue script brackets ('<' and '>').");
      }
      else if(nextCloseIndex < nextOpenIndex)
      {
         T_T("Found extra '>' character in dialogue line."
            " Please balance your dialogue script brackets ('<' and '>').");
      }
      else if(nextOpenIndex < closeIndex)
      {
         T_T("Found nested '<' character in dialogue line."
            " Please revise the line to remove nested brackets ('<' and '>').");
      }
      else
      {
         openIndex = nextOpenIndex;
         closeIndex = nextCloseIndex;
         m_openScriptBrackets.push(openIndex);
         m_closeScriptBrackets.push(closeIndex);
         DEBUG("Found embedded script starting at %d, ending at %d", openIndex, closeIndex);
      }
   }
}

DialogueEntry::~DialogueEntry()
{
   if(task)
   {
      task->complete();
   }
}

bool DialogueEntry::getNextBracketPair(unsigned int& openIndex, unsigned int& closeIndex) const
{
   if(m_openScriptBrackets.empty())
   {
      return false;
   }

   openIndex = m_openScriptBrackets.front();
   closeIndex = m_closeScriptBrackets.front();

   return true;
}

std::string DialogueEntry::removeNextScriptString()
{
   unsigned int openIndex, closeIndex;
   getNextBracketPair(openIndex, closeIndex);

   m_openScriptBrackets.pop();
   m_closeScriptBrackets.pop();

   const int scriptLength = closeIndex - openIndex;
   std::string script = text.substr(openIndex + 1, scriptLength - 1);
   text.replace(openIndex, scriptLength + 1, "");

   DEBUG("Extracting script %s, leaving dialogue %s", script.c_str(), text.c_str());
   return script;
}

