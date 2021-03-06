/*--------------- P r o g 6 . c p p ---------------

by:   George Cheney
      16.322 Data Structures
      ECE Department
      UMASS Lowell

PURPOSE
This is an interactive polygon statistics program. A polygon is entered the keyboard.
Then, upon command, the program will display the circumference and area of
the polygon.

DEMONSTRATES
Singly Linked Lists

CHANGES          
03-06-2018 gpc - Created for 16.322
03-21-2018 ers - wrote ShowArea function
*/
#include <iostream>
#include <cmath>

using namespace std;

#include "Point.h"
#include "LinkedList.h"

//----- c o n s t a n t    d e f i n i t i o n s -----

// Command prompt
const char CmdPrompt[] = "\n>";  // Prompt user to enter a command.

// Command Letters
const char AreaCmd =    'A';     // Compute and display the area of the polygon
                                 // and its triangles.
const char CircumCmd =  'C';     // Compute and display the circumference of the polygon.
const char DeleteCmd =  'D';     // Delete the current point from the polygon.
const char EraseCmd =   'E';     // Make the polygon empty
const char ForwardCmd = 'F';     // Move the current entry forward one step.
const char InsertCmd =  'I';     // Insert a new point in the polygon.
const char PrintCmd =   'P';     // Show the polygon on the display.
const char QuitCmd =    'Q';     // Quit
const char RewindCmd =  'R';     // Rewind the polygon to the first point.
const char SearchCmd =  'S';     // Find the next entry with a given x coordinate.
const char UpdateCmd =  'U';     // Update the current entry

const unsigned MinPolySides = 3;	// A polygon must have at least 3 sides.

//----- f u n c t i o n    p r o t o t y p e s -----

char GetCmd(List &polygon);
void InsertPoints(List &polygon);
void ClearPolygon(List &polygon);
void DisplayPolygon(List &polygon);
void ShowArea(List &polygon);
void ShowCircum(List &polygon);
bool ValidPoly(List &polygon);
void UpdateEntry(List &polygon);
void Search(List &polygon);

//--------------- m a i n ( ) ---------------

int main()
{
   List  polygon;    // The polygon list
   char cmd;         // The command letter

   // Repeatedly read a command from the keyboard and execute it.
   for (;;)
      {
      cmd = GetCmd(polygon); // Get the command letter.                    
      // If not empty command, execute it.
      if (cmd != ' ')
         {
         switch (toupper(cmd))
            {
            case AreaCmd:     // Display the areas.
               ShowArea(polygon);
               break;
            case CircumCmd:   // Display the circumference.
               ShowCircum(polygon);
               break;
            case UpdateCmd:   // Update the current entry.
	            UpdateEntry(polygon);
	            break;
            case InsertCmd:   // Insert a new point.
               InsertPoints(polygon);
               break;
            case EraseCmd:    // Clear the entire polygon.
               ClearPolygon(polygon);
               break;
            case DeleteCmd:   // Delete the current point.
               if (!polygon.AtEnd())
                  polygon.Delete();
               if (polygon.Empty())
                  cout << "The polygon is empty." << endl;
               break;
            case PrintCmd:    // Display the polygon.
               DisplayPolygon(polygon);
               break;
            case ForwardCmd:  // Advance to the next point.
               if (!polygon.AtEnd())
                  polygon.Skip();
               if (polygon.AtEnd())
                  cout << "The polygon is at the end." << endl;
               break;
            case RewindCmd:   // Rewind making the first point current.
               polygon.Rewind();
               break;
            case SearchCmd:   // Find the next point with a given x coordinate.
               Search(polygon);
               break;
            case QuitCmd:     // Terminate execution.
               return 0;
            case ' ':         // Empty command; do nothing.
               break;
            default:          // Bad command; display error message.
               cout << "*** Error: Unknown Command" << endl;
               break;
            }
      }
    }
  return 0;
}

/*--------------- G e t C m d ( ) ---------------

PURPOSE
Accept a command from the keyboard.

INPUT PARAMETERS
polygon      -- the polygon list.

RETURN VALUE
The command letter.
*/
char GetCmd(List &polygon)
{
   // Display the current point before accepting each command.
   if (!polygon.AtEnd())
      {
      // Display the current item.
      cout << "\nCURRENT ITEM" << endl;
      polygon.CurrentEntry().Show();
      cout << endl;
      }

   // Prompt for a new command.
   cout << CmdPrompt;

   // initialize command empty.
   char cmd = ' ';

   // Read the command letter from the keyboard.
   if (cin.peek() != '\n')
      cmd = cin.get();    // Command line

   cin.ignore(INT_MAX, '\n');

   return cmd;
}

/*--------------- I n s e r t P o i n t s ( ) ---------------

PURPOSE
Insert a new point in the polygon before the current point.

INPUT PARAMETERS
polygon  -- the polygon list.
*/
void InsertPoints(List &polygon)
{
  Point             point;    // New polygon point
  
   for (;;)
      {
      // Read the new point and insert it into the polygon.
      cout << "Enter point: ";
      point.Get();
      if (!point.Defined())
         return;
      polygon.Insert(point);
      }   
}

/*--------------- U p d a t e E n t r y( ) ---------------

PURPOSE
Modify the current entry in the polygon.

INPUT PARAMETERS
polygon  -- the polygon list.
*/
void UpdateEntry(List &polygon)
{
   Point point;                // New point value
  
   // If at end and there is no current entry, say so.
	if (polygon.AtEnd())
		{
		cout << "***Error: There is no current entry." << endl;
		return;
		}

   // Read the new point and insert it into the polygon.
   cout << "Enter point: ";
   point.Get();
   if(point.Defined())
      polygon.Update(point);
}


/*--------------- D i s p l a y P o l y g o n ( ) ---------------

PURPOSE
Display a polygon from beginning to end.

INPUT PARAMETERS
polygon  -- the polygon to be displayed.
*/
void DisplayPolygon(List &polygon)
{
   // If the polygon is empty, say so.
   if (polygon.Empty())
      cout << "The polygon is empty." << endl;
   else
      {
      cout << "\nPOLYGON DEFINITION" << endl;

      // Start at the beginning.
      polygon.Rewind();
      
      // Keep displaying until the end.
      while (!polygon.AtEnd())
        {
        // Display the current entry..
        polygon.CurrentEntry().Show();
        cout << endl;

        // Move to the next entry.
        polygon.Skip();
        }
  
      cout << endl;
      }

   // Rewind when done.
   polygon.Rewind();
}

/*--------------- C l e a r P o l y g o n ( ) ---------------

PURPOSE
Make the polygon empty.

INPUT PARAMETERS
polygon  -- the polygon list.
*/
void ClearPolygon(List &polygon)
{
  // Delete vertexes until empty.
  polygon.Rewind();
  while (!polygon.Empty())
    polygon.Delete();
}


/*--------------- V a l i d P o l y ( ) ---------------

PURPOSE
Make sure that there at least 3 points.

INPUT PARAMETERS
polygon  -- the polygon list.

RETURN VALUE
true if there are at least 3 points,
false otherwise
*/
bool ValidPoly(List &polygon)
{
	// Rewind to the first point.
	polygon.Rewind();

	// Make sure that there are at least 3 points.
	for (unsigned i=0; i<MinPolySides; i++)
		{
		if (polygon.AtEnd())
			{
			// If not valid, say so.
			cout << "*** ERROR: At least " << MinPolySides << " points are needed to define a polygon." << endl;
			return false;
			}
		polygon.Skip();
		}

	// Indicate valid.
	return true;
}

/*--------------- S e a r c h ( ) ---------------

PURPOSE
Starting at the current point, find the first point with the given x coordinate.

INPUT PARAMETERS
polygon  -- the polygon list.
*/
void Search(List &polygon)
{
   // Get the desired x.
   cout << "X Coordinate: ";
   double x;   // Desired x coordinate

   // Read in desired x coordinate.
   if (cin.peek() != '\n')
      {
      cin >> x;

      // Skip ahead to the desired x.
      for (;;)
         {
         // If at end and not found, return.
         if (polygon.AtEnd())
            {
            cout << "Polygon at end\n";
            break;
            }
         // If this entry is the one then done.
         if (polygon.CurrentEntry().X() == x)
            break;
         // Keep looking.
         polygon.Skip();
         }
      }
   cin.ignore(INT_MAX, '\n');
}

/*--------------- S h o w C i r c u m ( ) ---------------

PURPOSE
Show on the screen the the circumference of the polygon.

INPUT PARAMETERS
polygon  -- the polygon list.
*/
void ShowCircum(List &polygon)
{
   // Check for valid polygon.
   if (!ValidPoly(polygon))
	   return;

   // Rewind the current entry.
   polygon.Rewind();

   // Save the first point for computing the length of the last side.
   Point p0 = polygon.CurrentEntry();

   // Prepare to accumulate the circumference.
   double circum = 0;

   // Starting point of the first side
   Point p1 = p0;

   // Repeatedly add the lengths of the polygon sides.
   while (!polygon.AtEnd())
      {
	   // Advance to the next point.
      polygon.Skip();

	   // If no more points, done
      if (polygon.AtEnd())
	      // Accumulate the length of the last side.
         circum += p1.Length(p0);
      else
         {
         // Get the next side's end point.
         Point p2 = polygon.CurrentEntry();

		   // Accumulate the length of the next side.
         circum += p1.Length(p2);

		   // Last end point is next starting point.
		   p1 = p2;
         }
      }

   // Show the circumference.
   cout << "Circumference = " << circum << endl;
}

/*--------------- S h o w A r e a ( ) ---------------

PURPOSE
Show on the screen the areas of the triangles comprising the
polygon and show the area of the entire polygon.

INPUT PARAMETERS
polygon  -- the polygon list.
*/
void ShowArea(List &polygon)
{
   // Make sure the polygon is valid.
	if (!ValidPoly(polygon))
		return;

	// Rewind to the first point.
	polygon.Rewind();

	unsigned numSides = 0; //count of sides

	//count number of sides
	while (!polygon.AtEnd())
	{
		numSides += 1;
		polygon.Skip();
	}

	unsigned numTri = numSides - 2; //number of triangles

	double totalArea = 0; //totalArea

	polygon.Rewind(); 
	double area = 0; //area of triangle
	unsigned count = 1; //count which triangle is be analyzed
	Point start = polygon.CurrentEntry(); //starting point of polygon/triangle

	//for first triangle
	polygon.Skip();
	Point secondPoint = polygon.CurrentEntry(); //second point of triangle
	polygon.Skip();
	Point thirdPoint = polygon.CurrentEntry(); //third point of triangle

	//three sides of triangle and s variable
	double a = start.Length(secondPoint);
	double b = secondPoint.Length(thirdPoint);
	double c = thirdPoint.Length(start);
	double s = (a + b + c) / 2;

	//area calculation
	area += sqrt(s*(s - a)*(s - b)*(s - c));
	totalArea += area;

	cout << "Triangle 1 area = " << area << "\n";

	//loop for tracking which sides to be used in calculating triangle area.
	for (unsigned j = numTri - 1; j > 0; j--)
	{
		area = 0;
		count += 1;
		secondPoint = thirdPoint;
		polygon.Skip();
		Point thirdPoint = polygon.CurrentEntry(); //third point of triangle

		a = start.Length(secondPoint);
		b = secondPoint.Length(thirdPoint);
		c = thirdPoint.Length(start);
		s = (a + b + c) / 2;

		area += sqrt(s*(s - a)*(s - b)*(s - c));
		totalArea += area;

		cout << "Triangle " << count << " area = " << area << "\n";
	}

	cout << "Polygon area = " << totalArea << "\n";
}
