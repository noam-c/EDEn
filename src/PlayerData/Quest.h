/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef QUEST_H
#define QUEST_H

#include <string>
#include <map>

namespace Json
{
   class Value;
};

/**
 * A quest is a task assigned to the player to complete, and is used as a device to help move the game plot forward.
 * Quest objects track all the data needed to allow scripts (and the player) to track progress. This data includes
 * things like description, completion state, and whether or not the quest is optional. The quest structure can be augmented
 * with additional information later on as needed.
 *
 * An example quest could be something like:
 *
 * - Talk to some NPC.
 *
 * Where the specified NPC will mark the quest as completed.
 *
 * Quests can get more complex, involving subquests that represent the natural breakdown of quests into a series of smaller tasks:
 *
 * - Help an NPC hunt monsters.
 *    - Kill 5 goblins
 *    - Kill 2 kobolds
 *    - Kill 1 giant
 *
 * As a result, subquests can be used to record progress through an overarching quest.
 *
 * @author Noam Chitayat
 */
class Quest
{
   /**
    * Represents a table of quests, indexed by their names.
    */
   typedef std::map<std::string, Quest*> QuestLog;

   
   /**
    * The name of the quest.
    */
   std::string name;
   
   /**
    * An optional description of the quest.
    */
   std::string description;
   
   /**
    * True iff the quest is considered optional (i.e. a sidequest)
    */
   bool optional;
   
   /**
    * True iff the quest has been completed.
    */
   bool completed;

   /**
    * The subquests required by this quest.
    */
   QuestLog subquests;

   
   public:   
      /**
       * Constructor.
       *
       * @param name The name of the quest
       * @param description The description of the quest (empty by default)
       * @param optional True iff the quest is optional (false by default)
       * @param completed Whether or not the quest has already been completed (false by default)
       */
      Quest(const std::string& name, const std::string& description = "", bool optional = false, bool completed = false);

      /**
       * Constructor. Initializes quest by deserializing JSON data.
       *
       * @param questJson The JSON data to load from.
       */
      Quest(const Json::Value& questJson);
   
      /**
       * Destructor.
       * Recursively cleans up all subquests.
       */
      ~Quest();

      /**
       * Deserializes JSON data into the Quest.
       *
       * @param questJson The JSON data to load from.
       */
      void load(const Json::Value& questJson);
   
      /**
       * Serializes the quest and its subquests into JSON.
       *
       * @return The serialized JSON for this quest.
       */
      Json::Value serialize() const;

      /**
       * Add a subquest to this quest's log.
       */
      void addQuest(Quest* quest);
   
      /**
       * Recursively searches for the subquest on the path specified.
       *
       * @param questPath A "/"-delimited path from the quest to the requested subquest.
       * @return The quest at the path specified by questPath, or NULL if it could not be found.
       */
      Quest* getQuest(const std::string& questPath) const;
   
      /**
       * @return true iff the quest has been completed.
       */
      bool isCompleted() const;
   
      /**
       * Completes the quest.
       */
      void complete();

      /**
       * @return The name of the quest.
       */
      std::string getName() const;
   
      /**
       * @return The description of the quest.
       */
      std::string getDescription() const;
};

#endif
