<?php

namespace App\Entities;

use App\Entities\Model;
use Illuminate\Support\Facades\DB;

class InformationColumns extends Model {

    public function getTables($esquema, $tabela) {
        return $this->select('col.table_name', 'col.column_name', 'col.is_nullable', 'col.data_type', 'col.character_maximum_length', 'col.column_default', 'kcu.constraint_name', 'pgd.description', DB::raw('obj_description(stt.relid) as table_description'))
                    ->from('information_schema.columns as col')
                    ->leftJoin('information_schema.table_constraints as tc', function($join) {
                        $join->on('tc.table_catalog' ,'=', 'col.table_catalog');
                        $join->on('tc.table_schema' ,'=', 'col.table_schema');
                        $join->on('tc.table_name' ,'=', 'col.table_name');
                        $join->on('tc.constraint_type', '=', DB::raw('\'PRIMARY KEY\''));
                    })
                    ->leftJoin('information_schema.key_column_usage as kcu', function($join){
                        $join->on('kcu.table_catalog', '=', 'tc.table_catalog');
                        $join->on('kcu.table_schema', '=', 'tc.table_schema');
                        $join->on('kcu.table_name', '=', 'tc.table_name');
                        $join->on('kcu.column_name', '=', 'col.column_name');
                    })
                    ->leftJoin('pg_catalog.pg_statio_all_tables as stt', function($join){
                        $join->on('stt.schemaname', '=', 'col.table_schema');
                        $join->on('stt.relname', '=', 'col.table_name');
                    })
                    ->leftJoin('pg_catalog.pg_description as pgd', function($join){
                        $join->on('pgd.objsubid', '=', 'col.ordinal_position');
                        $join->on('pgd.objoid', '=', 'stt.relid');
                    })
                    ->where(['col.table_schema' => DB::raw('\''.$esquema.'\''), 'col.table_name' => DB::raw('\''.$tabela.'\'')])
                    ->get()->toArray();
    }

    public function getPrimaryKey($esquema, $tabela) {
        return $this->select('kcu.column_name')
                    ->from('information_schema.tables as t')
                    ->leftJoin('information_schema.table_constraints as tc', function($join){
                        $join->on('tc.table_catalog', '=', 't.table_catalog');
                        $join->on('tc.table_schema', '=', 't.table_schema');
                        $join->on('tc.table_name', '=', 't.table_name');
                        $join->on('tc.constraint_type', '=', DB::raw('\'PRIMARY KEY\''));
                    })
                    ->leftJoin('information_schema.key_column_usage as kcu', function($join){
                        $join->on('kcu.table_catalog', '=', 'tc.table_catalog');
                        $join->on('kcu.table_schema', '=', 'tc.table_schema');
                        $join->on('kcu.table_name', '=', 'tc.table_name');
                        $join->on('kcu.constraint_name', '=', 'tc.constraint_name');
                    })
                    ->where([
                        't.table_schema' => DB::raw('\''.$esquema.'\''),
                        't.table_name' => DB::raw('\''.$tabela.'\'')
                    ])
                    ->get()->toArray()[0]['column_name'];
    }

    protected function getDoctrineConnection()
    {
        return \DB::connection()->getDoctrineConnection();
    }

    public function fetch($sql)
    {
        $stmt = $this->getDoctrineConnection()->query($sql)->fetchAll();
        return $stmt;
    }

}
