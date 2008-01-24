/***************************************************************************
 *   Copyright (C) 2005-2007 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MMSCONFIGQUERIES_H_
#define MMSCONFIGQUERIES_H_

/****************************************************************************************/
/*                                                                                      */
/* All queries used in the PluginDAO class                                              */
/*                                                                                      */
/****************************************************************************************/
#define PLUGINDAO_SAVE(ID, Name, Title, Description, Filename, Path, Active, Icon, Icon_s, Icon_small, Icon_small_s) \
        "insert into Plugins(PluginTypeID,PluginName,PluginTitle,PluginDescription,Filename,PluginPath,Active,Icon,SelectedIcon,SmallIcon,SmallSelectedIcon) values('" \
        + ID + "','"          \
        + Name + "','"        \
        + Title + "','"       \
        + Description + "','" \
        + Filename + "','"    \
        + Path + "','"        \
        + Active + "','"      \
        + Icon + "','"        \
        + Icon_s + "','"      \
        + Icon_small + "','"  \
        + Icon_small_s + "')"

#define PLUGINDAO_UPDATE(Filename, Active, Description, ID) \
        "update Plugins set Filename='" + Filename + "',"   \
        + "Active='" + Active + "',"                        \
        + "PluginDescription='" + Description + "' " +      \
        "where ID = '" + ID + "'"

#define PLUGINDAO_FIND_ALL_ACTIVE_PLUGINS \
		"select Plug.ID, Plug.PluginName, Plug.PluginTitle, Plug.PluginDescription, Plug.Filename, Plug.PluginPath, Plug.Active, Plug.Icon, Plug.SelectedIcon, Plug.SmallIcon, Plug.SmallSelectedIcon, Plug.PluginTypeID, Plug.CategoryID, Cat.CategoryName, PlugType.PluginTypeName from Plugins Plug left join Category Cat ON Cat.ID = Plug.CategoryID left join PluginTypes PlugType ON PlugType.ID = Plug.PluginTypeID where Plug.Active = 'Y'"

#define PLUGINDAO_FIND_ALL_PLUGINS	 \
	    "select Plug.ID, Plug.PluginName, Plug.PluginTitle, Plug.PluginDescription, Plug.Filename, Plug.PluginPath, Plug.Active, Plug.Icon, Plug.SelectedIcon, Plug.SmallIcon, Plug.SmallSelectedIcon, Plug.PluginTypeID, Plug.CategoryID, Cat.CategoryName, PlugType.PluginTypeName from Plugins Plug left join Category Cat ON Cat.ID = Plug.CategoryID left join PluginTypes PlugType ON PlugType.ID = Plug.PluginTypeID" 

#define PLUGINDAO_F_PLUGIN_BY_NAME(Name) \
	    "select Plug.ID, Plug.PluginName, Plug.PluginTitle, Plug.PluginDescription, Plug.Filename, Plug.PluginPath, Plug.Active, Plug.Icon, Plug.SelectedIcon, Plug.SmallIcon, Plug.SmallSelectedIcon, Plug.PluginTypeID, Plug.CategoryID, Cat.CategoryName, PlugType.PluginTypeName from Plugins Plug left join Category Cat ON Cat.ID = Plug.CategoryID left join PluginTypes PlugType ON PlugType.ID = Plug.PluginTypeID where Plug.PluginName = '" + Name + "';"

#define PLUGINDAO_F_PLUGIN_BY_ID(ID) \
    	"select Plug.ID, Plug.PluginName, Plug.PluginTitle, Plug.PluginDescription, Plug.Filename, Plug.PluginPath, Plug.Active, Plug.Icon, Plug.SelectedIcon, Plug.SmallIcon, Plug.SmallSelectedIcon, Plug.PluginTypeID, Plug.CategoryID, Cat.CategoryName, PlugType.PluginTypeName from Plugins Plug left join Category Cat ON Cat.ID = Plug.CategoryID left join PluginTypes PlugType ON PlugType.ID = Plug.PluginTypeID where Plug.ID = " + ID

#define PLUGINDAO_F_ACTIVE_PLUGINS_BY_CATEGORY(CATEGORY) \
	    "select Plug.*,Cat.CategoryName,Types.PluginTypename from Plugins Plug left join Category Cat ON Cat.CategoryName ='" + CATEGORY + "' left join PluginTypes Types ON Types.ID = Plug.PluginTypeID WHERE Plug.CategoryID = Cat.ID and Plug.Active = 'Y'" 

#define PLUGINDAO_F_ALL_PLUGINS_BY_CATEGORY(CATEGORY) \
	    "select Plug.*,Cat.CategoryName,Types.PluginTypename from Plugins Plug left join Category Cat ON Cat.CategoryName ='" + CATEGORY + "' left join PluginTypes Types ON Types.ID = Plug.PluginTypeID WHERE Plug.CategoryID = Cat.ID" 

#define PLUGINDAO_F_ACTIVE_PLUGINS_BY_TYPE(TYPE) \
    	"select Plug.*,Cat.CategoryName,Types.PluginTypeName from Plugins Plug left join Category Cat ON Cat.ID  = Plug.CategoryID left join PluginTypes Types ON Types.ID = Plug.PluginTypeID where Types.PluginTypeName = '" + TYPE + "' and Plug.Active = 'Y'"

#define PLUGINDAO_F_ALL_PLUGINS_BY_TYPE(TYPE) \
    	"select Plug.*,Cat.CategoryName,Types.PluginTypeName from Plugins Plug left join Category Cat ON Cat.ID  = Plug.CategoryID left join PluginTypes Types ON Types.ID = Plug.PluginTypeID where Types.PluginTypeName = '" + TYPE + "'"

/****************************************************************************************/
/*                                                                                      */
/* All queries used in the PluginPropertiesDAO class                                    */
/*                                                                                      */
/****************************************************************************************/
#define PLUGINPROPERTYDAO_FIND_ALL_PLUGIN_PROPERTIES_BY_PLUGIN(ID) \
		"select * from PluginProperties where PluginID = " + ID

#endif /*MMSCONFIGQUERIES_H_*/
