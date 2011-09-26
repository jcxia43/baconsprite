#include "CellCoverage.h"

namespace BaconSprite
{
    CellCoverage::CellCoverage(Cell* cells,
                               int rows,
                               int columns,
                               int coverageWidth,
                               int coverageHeight)
    {
        // * Calculate row overage
        this->RowCoverage = new int[rows];
        
        int row = 0;
        while (row < rows)
        {
            int combined = cells[row * columns].Height,
                count = 1;
            
            while ((combined < coverageHeight) &&
                   (count > 0))
            {
                if (row + count >= rows)
                {
                    count = 0;
                    break;
                }
                
                combined += cells[(row + count) * columns].Height;
                count++;
            }
            
            this->RowCoverage[row] = count;
            
            row++;
        }
        
        // * Calculate column coverage
        this->ColumnCoverage = new int[columns];
        
        int column = 0;
        while (column < columns)
        {
            int combined = cells[column].Width,
                count = 1;
            
            while ((combined < coverageWidth) &&
                   (count > 0))
            {
                if (column + count >= columns)
                {
                    count = 0;
                    break;
                }
                
                combined += cells[column + count].Width;
                count++;
            }
            
            this->ColumnCoverage[column] = count;
            
            column++;
        }
        
        // * Determine boundaries
        row = rows;
        RowBoundary = 0;
        while (row--)
            if (RowCoverage[row] > 0)
            {
                RowBoundary = row + 1;
                break;
            }
        
        column = columns;
        ColumnBoundary = 0;
        while (column--)
            if (ColumnCoverage[column] > 0)
            {
                ColumnBoundary = column + 1;
                break;
            }
    }
    
    CellCoverage::~CellCoverage()
    {
        delete this->RowCoverage;
        delete this->ColumnCoverage;
    }
}