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
import { DBCKeyCell , DBCIntCell , DBCFloatArrayCell} from '../DBCCell'
import { int , float} from '../../primitives'
import { PrimaryKey } from '../../table/PrimaryKey'

 /**
  * Main row definition
  * - Add column comments to the commented getters below
  * - Add file comments to DBCFiles.ts
  */
export class ObjectEffectModifierRow extends DBCRow<ObjectEffectModifierCreator,ObjectEffectModifierQuery> {
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
    get InputType() { return new DBCIntCell(this,this.buffer,this.offset+4)}
    
    /**
     * No comment (yet!)
     */
    get MapType() { return new DBCIntCell(this,this.buffer,this.offset+8)}
    
    /**
     * No comment (yet!)
     */
    get OutputType() { return new DBCIntCell(this,this.buffer,this.offset+12)}
    
    /**
     * No comment (yet!)
     */
    get Param() { return new DBCFloatArrayCell(this,4,this.buffer,this.offset+16)}
    
    /**
     * Creates a clone of this row with new primary keys.
     * 
     * Cloned rows are automatically added at the end of the DBC file.
     */ 
    clone(ID : int, c? : ObjectEffectModifierCreator) : this {
        return this.cloneInternal([ID],c);
    }
}

/**
 * Used for object creation (Don't comment these)
 */
export type ObjectEffectModifierCreator = {
    InputType?: int
    MapType?: int
    OutputType?: int
    Param?: float
}

/**
 * Used for queries (Don't comment these)
 */
export type ObjectEffectModifierQuery = {
    ID? : Relation<int>
    InputType? : Relation<int>
    MapType? : Relation<int>
    OutputType? : Relation<int>
    Param? : Relation<float>
}

/**
 * Table definition (specifies arguments to 'add' function)
 * - Add file comments to DBCFiles.ts
 */
export class ObjectEffectModifierDBCFile extends DBCFile<
    ObjectEffectModifierCreator,
    ObjectEffectModifierQuery,
    ObjectEffectModifierRow> {
    add(ID : int, c? : ObjectEffectModifierCreator) : ObjectEffectModifierRow {
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
export const DBC_ObjectEffectModifier = new ObjectEffectModifierDBCFile(
    'ObjectEffectModifier',
    (table,buffer,offset)=>new ObjectEffectModifierRow(table,buffer,offset))