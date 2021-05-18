/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General  License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General  License for more details.

You should have received a copy of the GNU Lesser General  License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

#import <Cocoa/Cocoa.h>

// Here is our controller for the NSTableView it provides the data to the table.
@interface myTableViewController : NSObject
{
	NSMutableArray * tableArray;
}

// just returns the item for the right row
- (id)tableView:(NSTableView *) aTableView 
		objectValueForTableColumn:(NSTableColumn *) aTableColumn
		row:(int) rowIndex;

// just returns the number of items we have.
- (int)numberOfRowsInTableView:(NSTableView *)aTableView;

// Add an entry to our array
- (void)addEntry:(NSString *)entryText;

// Update an entry in our array
- (void)updateEntry: (NSString *) index : (NSString *) entryText;

// Get name at index
- (NSString *)getEntry:(int)index;

// Clear our array
- (void)clearArray;

// Create the class, I heared tale of an init() function that is like a constructor
// TODO: Use Object-C's real constructor...
- (void)create;

// Cleanup the data we used, also a dealloc() function?
// TODO: Use Object-C's real destructor
- (void)cleanup;
@end

// Our window definition
@interface OgreConfigWindowController : NSWindowController {
	// these hold our objects from the NIB file
    id renderSystemPopup;
	id messageTextField;
    id optionsPopUp;
	id optionsTableView;
	    
    BOOL done;
	
	// Our data provider to the table
	myTableViewController* tableDataSource;
	// The current available options
	NSMutableDictionary* optionData;
	// Our current settings
	NSMutableDictionary* currentSettings;
}

// The callback method for when the index changes in NSTableView
- (void)tableViewSelectionDidChange:(NSNotification *)aNotification;

// The callback for our optionPopUp NSPopupButton
- (void) optionPopUpAction:(id) sender;

- (void)prepareWindow;
- (void)displayWindow;
- (void)destroyWindow;
- (BOOL)getReadyFlag;
- (void)setMessage: (NSString *)message;
- (void)addRenderSystem: (NSString *)message;
- (int)getRenderSystem;
- (void)addOption: (NSString *)name;
- (void)populateOptionPopup: (NSString *)name;
- (void)addOptionKey: (NSString *) optionKey andEntry: (NSString *) optionString;
- (void)addOptionTable: (NSString *)optionName;
- (NSMutableDictionary*)getCurrentSettings;
- (void)addCurrentSettings: (NSString *) key andVal: (NSString *) valString; 

-(void)okClicked: (id)sender;
-(void)cancelClicked: (id)sender;
@end

