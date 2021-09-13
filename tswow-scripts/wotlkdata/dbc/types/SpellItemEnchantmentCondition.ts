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
import { DBCKeyCell , DBCByteArrayCell , DBCIntArrayCell} from '../DBCCell'
import { int , byte} from '../../primitives'
import { PrimaryKey } from '../../table/PrimaryKey'

 /**
  * Main row definition
  * - Add column comments to the commented getters below
  * - Add file comments to DBCFiles.ts
  */
export class SpellItemEnchantmentConditionRow extends DBCRow<SpellItemEnchantmentConditionCreator,SpellItemEnchantmentConditionQuery> {
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
    get Lt_OperandType() { return new DBCByteArrayCell(this,5,this.buffer,this.offset+4)}

    /**
     * No comment (yet!)
     */
    get Lt_Operand() { return new DBCIntArrayCell(this,5,this.buffer,this.offset+9)}

    /**
     * No comment (yet!)
     */
    get Operator() { return new DBCByteArrayCell(this,5,this.buffer,this.offset+29)}

    /**
     * No comment (yet!)
     */
    get Rt_OperandType() { return new DBCByteArrayCell(this,5,this.buffer,this.offset+34)}

    /**
     * No comment (yet!)
     */
    get Rt_Operand() { return new DBCIntArrayCell(this,5,this.buffer,this.offset+39)}

    /**
     * No comment (yet!)
     */
    get Logic() { return new DBCByteArrayCell(this,5,this.buffer,this.offset+59)}

    /**
     * Creates a clone of this row with new primary keys.
     *
     * Cloned rows are automatically added at the end of the DBC file.
     */
    clone(ID : int, c? : SpellItemEnchantmentConditionCreator) : this {
        return this.cloneInternal([ID],c);
    }
}

/**
 * Used for object creation (Don't comment these)
 */
export type SpellItemEnchantmentConditionCreator = {
    Lt_OperandType?: byte
    Lt_Operand?: int
    Operator?: byte
    Rt_OperandType?: byte
    Rt_Operand?: int
    Logic?: byte
}

/**
 * Used for queries (Don't comment these)
 */
export type SpellItemEnchantmentConditionQuery = {
    ID? : Relation<int>
    Lt_OperandType? : Relation<byte>
    Lt_Operand? : Relation<int>
    Operator? : Relation<byte>
    Rt_OperandType? : Relation<byte>
    Rt_Operand? : Relation<int>
    Logic? : Relation<byte>
}

/**
 * Table definition (specifies arguments to 'add' function)
 * - Add file comments to DBCFiles.ts
 */
export class SpellItemEnchantmentConditionDBCFile extends DBCFile<
    SpellItemEnchantmentConditionCreator,
    SpellItemEnchantmentConditionQuery,
    SpellItemEnchantmentConditionRow> {
    add(ID : int, c? : SpellItemEnchantmentConditionCreator) : SpellItemEnchantmentConditionRow {
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
export const DBC_SpellItemEnchantmentCondition = new SpellItemEnchantmentConditionDBCFile(
    'SpellItemEnchantmentCondition',
    (table,buffer,offset)=>new SpellItemEnchantmentConditionRow(table,buffer,offset))
