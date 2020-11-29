/*
 * Copyright (C) 2020 tswow <https://github.com/tswow/>
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/* tslint:disable */
import { DBCRow } from '../DBCRow'
import { DBCFile } from '../DBCFile'
import { Relation } from '../../query/Relations'
import { DBCKeyCell , DBCIntCell , DBCLocCell} from '../DBCCell'
import { int , loc_constructor} from '../../primitives'
import { PrimaryKey } from '../../table/PrimaryKey'

 /**
  * Main row definition
  * - Add column comments to the commented getters below
  * - Add file comments to DBCFiles.ts
  */
export class AuctionHouseRow extends DBCRow<AuctionHouseCreator,AuctionHouseQuery> {
    /**
     * Primary Key
     *
     * No comment (yet!)
     */
    @PrimaryKey()
    get ID() { return new DBCKeyCell(this,this.buffer,this.offset+0)}
    
    /**
     * No comment (yet!)
     */
    get FactionID() { return new DBCIntCell(this,this.buffer,this.offset+4)}
    
    /**
     * No comment (yet!)
     */
    get DepositRate() { return new DBCIntCell(this,this.buffer,this.offset+8)}
    
    /**
     * No comment (yet!)
     */
    get ConsignmentRate() { return new DBCIntCell(this,this.buffer,this.offset+12)}
    
    /**
     * No comment (yet!)
     */
    get Name() { return new DBCLocCell(this,this.buffer,this.offset+16)}
    
    /**
     * Creates a clone of this row with new primary keys.
     * 
     * Cloned rows are automatically added at the end of the DBC file.
     */ 
    clone(ID : int, c? : AuctionHouseCreator) : this {
        return this.cloneInternal([ID],c);
    }
}

/**
 * Used for object creation (Don't comment these)
 */
export type AuctionHouseCreator = {
    FactionID?: int
    DepositRate?: int
    ConsignmentRate?: int
    Name?: loc_constructor
}

/**
 * Used for queries (Don't comment these)
 */
export type AuctionHouseQuery = {
    ID? : Relation<int>
    FactionID? : Relation<int>
    DepositRate? : Relation<int>
    ConsignmentRate? : Relation<int>
    Name? : Relation<string>
}

/**
 * Table definition (specifies arguments to 'add' function)
 * - Add file comments to DBCFiles.ts
 */
export class AuctionHouseDBCFile extends DBCFile<
    AuctionHouseCreator,
    AuctionHouseQuery,
    AuctionHouseRow> {
    add(ID : int, c? : AuctionHouseCreator) : AuctionHouseRow {
        return this.makeRow(0).clone(ID,c)
    }
    findById(id: number) {
        return this.fastSearch(id);
    }
}

/**
 * Table singleton (Object used by 'DBC' namespace)
 * - Add file comments to DBCFiles.ts
 */
export const DBC_AuctionHouse = new AuctionHouseDBCFile(
    'AuctionHouse',
    (table,buffer,offset)=>new AuctionHouseRow(table,buffer,offset))
