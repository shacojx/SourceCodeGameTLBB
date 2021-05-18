/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright © 2000-2002 The OGRE Team
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
#import "macOgreConfigWindowController.h"
#include "macOgreSDLConfig.h"



using namespace Ogre;

bool SDLConfig::display(void)
{
	// This dialog may be able to be done in a matter better than this, however it is now
	// a near spitting image of the windows dialog, and fully function. -Justin Walsh
	
    OgreConfigWindowController *config = [[OgreConfigWindowController alloc] 
                                            initWithWindowNibName: [ NSString stringWithCString: "config"]];
	
    // Get our window ready for display
	[config prepareWindow];
	
	// Load our previous config if we can
	Root::getSingleton().restoreConfig();
	
	// Set the renderSystem NsPopupButton's items
    RenderSystemList* renderers = Root::getSingleton().getAvailableRenderers();
    for (RenderSystemList::iterator pRend = renderers->begin();
            pRend != renderers->end(); pRend++)
    {
        [config addRenderSystem: [NSString stringWithCString: ((*pRend)->getName()).c_str()]];
    }
	
	// Get the selected renderSystem
	// TODO: If OS X where to ever have more than one rendersystem
	//		 this dailog would need to recheck the options for the correct display of said rendersystem
    int rendererIndex = [config getRenderSystem];
    if(rendererIndex < 0) {
        return false;
    }
	
	// Using our selected render system get the options
    RenderSystemList::iterator pRend =  renderers->begin();

    RenderSystem* renderer = pRend[rendererIndex];

    ConfigOptionMap options = renderer->getConfigOptions();
	
	// First lets just restore the options...
	ConfigOptionMap::iterator pOpt = options.begin();
	String strLine;
	while( pOpt!= options.end() )
	{
		strLine = pOpt->second.name + ": " + pOpt->second.currentValue;
		[config addOptionTable: [NSString stringWithCString: strLine.c_str()]];
		[config addCurrentSettings: [NSString stringWithCString: pOpt->second.name.c_str()] andVal: [NSString stringWithCString: pOpt->second.currentValue.c_str()]]; 
		++pOpt;
	}
	
    // Fill our NSMutableArray with our option map
	for (ConfigOptionMap::iterator it = options.begin(); 
		 it != options.end(); it++)
    {
		StringVector::iterator opt_it;
        for (opt_it = it->second.possibleValues.begin();
			 opt_it != it->second.possibleValues.end(); opt_it++)
        {
			[config addOptionKey: [NSString stringWithCString: (*opt_it).c_str()] andEntry: [NSString stringWithCString: it->second.name.c_str()]];
        }
	}
	
	[config displayWindow];
	
	// If user clicked canceled then exit
	if(![config getReadyFlag])
		return false;
	
	// Get the settings from the dialog and set the rendersystem options
	// Warning Object-C code :p
	NSMutableDictionary *mConfigOptions = [config getCurrentSettings];
	NSEnumerator *enumerator;
	enumerator = [mConfigOptions keyEnumerator];
	id key;
	
	while ((key = [enumerator nextObject]))
	{
			//NSLog(@"Set Config Options: %@ - %@", key, [mConfigOptions objectForKey: key]);
		renderer->setConfigOption( [key cString], [[mConfigOptions objectForKey: key] cString] );
    }
	
    // All done
    Root::getSingleton().setRenderSystem(renderer);
    Root::getSingleton().saveConfig();
	[config destroyWindow];
    [config release];
    return true;
}

// Create our NSTableDataSource for use as a data provider to our NSTableView.
@implementation myTableViewController : NSObject

// just returns the item for the right row
-(id) tableView:(NSTableView *) aTableView 
		objectValueForTableColumn:(NSTableColumn *) aTableColumn
		row:(int) rowIndex
{  
	return [tableArray objectAtIndex:rowIndex];
}

// just returns the number of items we have.
-(int) numberOfRowsInTableView:(NSTableView *)aTableView
{
	return [tableArray count];
}

// Add an entry to our array
-(void) addEntry:(NSString *)entryText
{
	[tableArray addObject: entryText];
}

// Find an array item by Text then replace it with new text
-(void) updateEntry: (NSString *) index : (NSString *) entryText
{
	// Lets be nice and do some checking for the user, so they can just pass the original name ofthe box...
	// i.e. if it is displaying FSAA: 4 lets just match the FSAA part :)
	NSString * search;
	NSRange myRange = [index rangeOfString: @":"];
	if( myRange.location == NSNotFound )
	{
		search = index;
	}
	else
	{
		search = [index substringToIndex: myRange.location];
	}

		//NSLog(@"UPDATE ENTRY FUNCTION:");
		//NSLog(@"SubString test search string: %@", search);
		//NSLog(@"SubString test index string: %@", index);

	for(unsigned int i = 0; i < [tableArray count]; i++)
	{
		NSString * arraySearch;
		NSRange myRange = [[tableArray objectAtIndex: i] rangeOfString: @":"];
		if( myRange.location == NSNotFound )
		{
			arraySearch = [tableArray objectAtIndex: i];
		}
		else
		{
			arraySearch = [[tableArray objectAtIndex: i] substringToIndex: myRange.location];
		}
		
		if( [arraySearch isEqualToString: search] )
		{
			[tableArray replaceObjectAtIndex: i withObject:entryText];
		}
	}
}

// Return our text entry from index
-(NSString *) getEntry:(int)index
{
	return [tableArray objectAtIndex: index];
}

// Clear our array
// TODO: there has to be a better way...
-(void) clearArray
{
	[tableArray release];
	tableArray = [[NSMutableArray alloc] init];
}

// TODO: Replace tihe the ture Object-C constructor Destructor calls
- (void)create
{
	tableArray = [[NSMutableArray alloc] init];
}

-(void) cleanup
{
	[tableArray release];
}
@end

@implementation OgreConfigWindowController : NSWindowController 

-(void) tableViewSelectionDidChange:(NSNotification *)aNotification
{
	NSTableView* tempObject = [aNotification object];
	NSString* curSelection = [tableDataSource getEntry: [tempObject selectedRow]];
	
	NSString * search;
	NSRange myRange = [curSelection rangeOfString: @":"];
	if( myRange.location == NSNotFound )
	{
		search = curSelection;
	}
	else
	{
		search = [curSelection substringToIndex: myRange.location];
	}
	
	[self populateOptionPopup: search];
	NSString * selectedIndex = [currentSettings objectForKey: search];
	[optionsPopUp selectItemWithTitle: selectedIndex];
	search = [search stringByAppendingString:@":"];
	[self setMessage: search];
}

// This is called when ever a new item is selected in our optionsPopUp
-(void) optionPopUpAction:(id) sender;
{
		//NSLog(@"PopupEntry Function:");
	NSString *curPop = [optionsPopUp titleOfSelectedItem];
		//NSLog(@"Curent PopUP Selection: %@", curPop);
	NSString *curMatch = [optionData objectForKey: curPop];
		//NSLog(@"Curent Matching Pair: %@", curMatch);
	NSString* newString = [curMatch stringByAppendingString:@": "];
	newString = [newString stringByAppendingString: curPop];
		//NSLog(@"CONCATINATED STRING: %@", newString);	
		
	[currentSettings setObject: curPop forKey: curMatch];
		
	[tableDataSource updateEntry: curMatch : newString];
	[optionsTableView reloadData];
}

-(void) prepareWindow {
	[self window];
	done = NO;
	tableDataSource = [[myTableViewController alloc] init];
	[tableDataSource create];
	
	// Register our class for the data source
	[optionsTableView setDataSource: tableDataSource];
	
	// Register the callback funtion for index change on our NSTableView
	[optionsTableView setDelegate: self];
	
	// Try and register a callback to our NSPopupButton
	[optionsPopUp setTarget: self];
	[optionsPopUp setAction:@selector(optionPopUpAction:)];
	
	optionData = [[NSMutableDictionary alloc] init];
	currentSettings = [[NSMutableDictionary alloc] init];
	[optionsPopUp removeAllItems]; 
	[renderSystemPopup removeAllItems];
}

-(void) displayWindow {
	[optionsTableView reloadData];
	[self showWindow: self];
	[[NSApplication sharedApplication]runModalForWindow: [self window]];
	[[self window] close];
}

-(void) destroyWindow {
	[tableDataSource cleanup];
	[tableDataSource release];
	[optionData release];
	[currentSettings release];
}

-(BOOL) getReadyFlag {
	return done;
}

-(void) setMessage: (NSString *)message {
    [messageTextField setStringValue: message]; 
}

-(void) addRenderSystem: (NSString *)message {
	[renderSystemPopup addItemWithTitle: message];
}

-(int) getRenderSystem {
	int selected;
	selected = [renderSystemPopup indexOfSelectedItem];
	return selected;
}

-(void) addOption: (NSString *)title {
    [optionsPopUp addItemWithTitle: title];
}

-(void) okClicked: (id)sender{
	done = YES;
	[[NSApplication sharedApplication]stopModal];
}

-(void) cancelClicked: (id)sender {
	done = NO;
	[[NSApplication sharedApplication]stopModal];
}

-(void) populateOptionPopup: (NSString *)name
{
	[optionsPopUp removeAllItems];
	
	NSMutableArray* sorted = [[NSMutableArray alloc] init];
	//NSSortDescriptor* sortDescriptor;
	NSEnumerator *enumerator;
	enumerator = [optionData keyEnumerator];
	id key;
	
	while ((key = [enumerator nextObject]))
	{
		if( [name isEqualToString: [optionData objectForKey: key]] )
		{ 
			//[self addOption: key];
			[sorted addObject:key];
		}
			//NSLog(@"%@ : %@", key, [optionData objectForKey:key]);
    }
	// Trying a simple sort on the data for the resolutions, better but not perfect
	// TODO: Really sort the data so it is in the proper order, at least now it is grouped and alphabetical.
	//[sorted sortUsingSelector:@selector(caseInsensitiveCompare:)];
    [sorted sortUsingSelector:@selector(localizedCompare:)];
	for( unsigned int i = 0; i < [sorted count]; i++)
	{
		[self addOption: [sorted objectAtIndex: i]];
	}
}

-(void) addOptionKey: (NSString *) optionKey andEntry: (NSString *) optionString
{
	// Key is the value for the option
	[optionData setObject: optionString forKey: optionKey];
}

-(void)addOptionTable: (NSString *)optionName
{
	[tableDataSource addEntry: optionName];
}

-(NSMutableDictionary*) getCurrentSettings
{
	return currentSettings;
}

-(void) addCurrentSettings: (NSString *) key andVal: (NSString *) valString
{
	[currentSettings setObject: valString forKey: key];
}

@end