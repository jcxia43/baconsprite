#include "CellSet.h"

namespace BaconSprite
{
    CellSet::CellSet(int width,
                     int height)
    :   _rows(1),
        _columns(1)
    {
        // Initialize a 1x1 cell array
        this->_cells = new Cell[1];
        
        this->_cells[0].Width = width;
        this->_cells[0].Height = height;
        
        this->_cells[0].Occupied = false;
    }
    
    CellSet::~CellSet()
    {
        delete this->_cells;
    }
    
    bool CellSet::Place(int width,
                        int height,
                        TestApproach approach,
                        int& leftOut,
                        int& topOut)
    {
        switch (approach)
        {
        case TestLeftFirst:
            return this->PlaceLeftFirst(width,
                                        height,
                                        leftOut,
                                        topOut);
        
        case TestTopFirst:
            return this->PlaceTopFirst(width,
                                       height,
                                       leftOut,
                                       topOut);
        }
    }
    
    inline bool CellSet::IsAvailable(CellCoverage& coverage,
                                     int width,
                                     int height,
                                     int offsetRow,
                                     int offsetColumn)
    {
        // * Define ranges
        int row = offsetRow,
            rowMax = offsetRow + coverage.RowCoverage[offsetRow],
            column = offsetColumn,
            columnMax = offsetColumn + coverage.ColumnCoverage[offsetColumn];

        while (row < rowMax)
        {
            column = offsetColumn;

            while (column < columnMax)
            {
                if (_cells[row * this->_columns + column].Occupied)
                    return false;

                column++;
            }

            row++;
        }

        return true;
    }
    
    inline void CellSet::Occupy(CellCoverage& coverage,
                                int width,
                                int height,
                                int offsetRow,
                                int offsetColumn)
    {
        // * Determine the rows and columns in question
        int rowMax = offsetRow + coverage.RowCoverage[offsetRow],
            columnMax = offsetColumn + coverage.ColumnCoverage[offsetColumn];
        
        // * Determine the width and height of the columns
        int rowsHeight = 0,
            columnsWidth = 0;

        for (int row = offsetRow; row < rowMax; row++)
            rowsHeight += _cells[row * this->_columns].Height;

        for (int column = offsetColumn; column < columnMax; column++)
            columnsWidth += _cells[column].Width;

        // * In the case that this is not complete coverage, create a new cell list
        if ((rowsHeight != height) ||
            (columnsWidth != width))
        {
            // Start out by determining the injection point of new rows and columns
            bool addRow = (rowsHeight != height),
                 addColumn = (columnsWidth != width);

            int rowInjection = rowMax,
                columnInjection = columnMax,
                newRows = _rows + (addRow ? 1 : 0),
                newColumns = _columns + (addColumn ? 1 : 0);

            // Just simply hard copy the cells for now
            Cell* newCells = new Cell[newRows * newColumns];

            for (int row = 0; row < newRows; row++)
            {
                int sourceRow = ((addRow) &&
                                 (row >= rowInjection) ?
                                 row - 1 :
                                 row);

                for (int column = 0; column < newColumns; column++)
                {
                    int sourceColumn = ((addColumn) && (column >= columnInjection) ? column - 1 : column);

                    newCells[row * newColumns + column] = _cells[sourceRow * this->_columns + sourceColumn];
                }
            }

            // If we're adding a row, resize the injected and the one before
            if (addRow)
            {
                for (int column = 0; column < newColumns; column++)
                {
                    newCells[(rowInjection - 1) * newColumns + column].Height -= rowsHeight - height;
                    newCells[rowInjection * newColumns + column].Height = rowsHeight - height;
                }
            }

            // .. same for columns
            if (addColumn)
            {
                for (int row = 0; row < newRows; row++)
                {
                    newCells[row * newColumns + columnInjection - 1].Width -= columnsWidth - width;
                    newCells[row * newColumns + columnInjection].Width = columnsWidth - width;
                }
            }
            
            // Finally, reassign the cells
            delete this->_cells;
            
            this->_cells = newCells;
            this->_rows = newRows;
            this->_columns = newColumns;
        }
        
        // * Then, mark as occupied
        for (int row = offsetRow; row < rowMax; row++)
        {
            for (int column = offsetColumn; column < columnMax; column++)
            {
                this->_cells[row * this->_columns + column].Occupied = true;
            }
        }
    }
    
    inline bool CellSet::PlaceLeftFirst(int width,
                                        int height,
                                        int& leftOut,
                                        int& topOut)
    {
        // * Determine cell coverage of the contextual rectangle
        CellCoverage coverage = CellCoverage(this->_cells,
                                             this->_rows,
                                             this->_columns,
                                             width,
                                             height);
        
        // * Iterate row and columns
        int positionTop = 0,
            positionLeft,
            row = 0,
            column;
        
        while (row < coverage.RowBoundary)
        {
            // Reset the left offset and iterate columns
            positionLeft = 0;
            column = 0;

            while (column < coverage.ColumnBoundary)
            {
                // Check if placement is feasible
                if (this->IsAvailable(coverage,
                                      width,
                                      height,
                                      row,
                                      column))
                {
                    // Occupy the space
                    this->Occupy(coverage,
                                 width,
                                 height,
                                 row,
                                 column);

                    // Return the placement rectangle
                    leftOut = positionLeft;
                    topOut = positionTop;
                    
                    return true;
                }

                // Increase the left offset
                positionLeft += _cells[column].Width;

                column++;
            }

            // Increase the top offset
            positionTop += _cells[row * this->_columns].Height;

            row++;
        }
        
        return false;
    }
    
    inline bool CellSet::PlaceTopFirst(int width,
                                       int height,
                                       int& leftOut,
                                       int& topOut)
    {
        // * Determine cell coverage of the contextual rectangle
        CellCoverage coverage = CellCoverage(this->_cells,
                                             this->_rows,
                                             this->_columns,
                                             width,
                                             height);
        
        // * Iterate row and columns
        int positionTop,
            positionLeft = 0,
            row,
            column = 0;
        
        while (column < coverage.ColumnBoundary)
        {
            // Reset the left offset and iterate columns
            positionTop = 0;
            row = 0;

            while (row < coverage.RowBoundary)
            {
                // Check if placement is feasible
                if (this->IsAvailable(coverage,
                                      width,
                                      height,
                                      row,
                                      column))
                {
                    // Occupy the space
                    this->Occupy(coverage,
                                 width,
                                 height,
                                 row,
                                 column);

                    // Return the placement rectangle
                    leftOut = positionLeft;
                    topOut = positionTop;
                    
                    return true;
                }

                // Increase the top offset
                positionTop += _cells[row * this->_columns].Height;

                row++;
            }
            
            // Increase the left offset
            positionLeft += _cells[column].Width;

            column++;
        }
        
        return false;
    }
}