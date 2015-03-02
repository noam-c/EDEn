#include "DialogueEntry.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_DIA_CONTR

DialogueEntry::DialogueEntry(DialogueEntryType type, const std::string& text, const std::shared_ptr<Task>& task) :
   m_task(task),
   type(type),
   text(text)
{
   parseTextScripts();
}

DialogueEntry::DialogueEntry(DialogueEntryType type, const std::string& text, const DialogueChoiceList& choices, const std::shared_ptr<Task>& task) :
   m_task(task),
   type(type),
   choices(choices),
   text(text)
{
   parseTextScripts();
}

DialogueEntry::DialogueEntry(DialogueEntryType type, const DialogueChoiceList& choices, const std::shared_ptr<Task>& task) :
   m_task(task),
   type(type),
   choices(choices)
{
   parseTextScripts();
}

void DialogueEntry::parseTextScripts()
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

void DialogueEntry::finish()
{
   if(m_task)
   {
      m_task->complete();
   }
}

bool DialogueEntry::choiceSelected(int choiceIndex)
{
   if(choiceIndex < 0 || choiceIndex >= choices.size())
   {
      DEBUG("Invalid choice index selected: %d", choiceIndex);
      return false;
   }
   
   DEBUG("Selected choice index: %d", choiceIndex);

   if(m_task)
   {
      m_task->complete(choiceIndex);
   }

   return true;
}

int DialogueEntry::getBeginningOfNextScript() const noexcept
{
   if(!m_openScriptBrackets.empty() && !m_closeScriptBrackets.empty())
   {
      return m_openScriptBrackets.front();
   }

   return -1;
}

std::string DialogueEntry::removeNextScriptString()
{
   std::string script;

   if(!m_openScriptBrackets.empty() && !m_closeScriptBrackets.empty())
   {
      const int openIndex = m_openScriptBrackets.front();
      const int closeIndex = m_closeScriptBrackets.front();

      const int scriptLength = closeIndex - openIndex;
      script = text.substr(openIndex + 1, scriptLength - 1);
      text.replace(openIndex, scriptLength + 1, "");

      DEBUG("Extracting script %s, leaving dialogue %s", script.c_str(), text.c_str());
      
      m_openScriptBrackets.pop();
      m_closeScriptBrackets.pop();
   }

   return script;
}
