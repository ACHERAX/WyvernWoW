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
import { DBCKeyCell , DBCIntCell , DBCIntArrayCell , DBCFloatArrayCell} from '../DBCCell'
import { int , float} from '../../primitives'
import { PrimaryKey } from '../../table/PrimaryKey'

 /**
  * Main row definition
  * - Add column comments to the commented getters below
  * - Add file comments to DBCFiles.ts
  */
export class LightfloatBandRow extends DBCRow<LightfloatBandCreator,LightfloatBandQuery> {
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
    get Num() { return new DBCIntCell(this,this.buffer,this.offset+4)}

    /**
     * No comment (yet!)
     */
    get Time() { return new DBCIntArrayCell(this,16,this.buffer,this.offset+8)}

    /**
     * No comment (yet!)
     */
    get Data() { return new DBCFloatArrayCell(this,16,this.buffer,this.offset+72)}

    /**
     * Creates a clone of this row with new primary keys.
     *
     * Cloned rows are automatically added at the end of the DBC file.
     */
    clone(ID : int, c? : LightfloatBandCreator) : this {
        return this.cloneInternal([ID],c);
    }
}

/**
 * Used for object creation (Don't comment these)
 */
export type LightfloatBandCreator = {
    Num?: int
    Time?: int
    Data?: float
}

/**
 * Used for queries (Don't comment these)
 */
export type LightfloatBandQuery = {
    ID? : Relation<int>
    Num? : Relation<int>
    Time? : Relation<int>
    Data? : Relation<float>
}

/**
 * Table definition (specifies arguments to 'add' function)
 * - Add file comments to DBCFiles.ts
 */
export class LightfloatBandDBCFile extends DBCFile<
    LightfloatBandCreator,
    LightfloatBandQuery,
    LightfloatBandRow> {
    add(ID : int, c? : LightfloatBandCreator) : LightfloatBandRow {
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
export const DBC_LightfloatBand = new LightfloatBandDBCFile(
    'LightFloatBand',
    (table,buffer,offset)=>new LightfloatBandRow(table,buffer,offset))
