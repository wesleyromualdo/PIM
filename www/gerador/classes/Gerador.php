<?php

/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 06/10/2015
 * Time: 11:40
 */
class Gerador
{

    public $stSchema;
    public $db;

    function __construct ()
    {
        $this->db = new cls_banco();
    }

    public function getTables ()
    {

        $sql = <<<DML
SELECT col.table_name,
       col.column_name,
       col.is_nullable,
       col.data_type,
       col.character_maximum_length,
       col.column_default,
       kcu.constraint_name,
       pgd.description,
       obj_description(stt.relid) as table_description
  FROM information_schema.columns col
    LEFT JOIN information_schema.table_constraints tc ON (tc.table_catalog = col.table_catalog
                                                          AND tc.table_schema = col.table_schema
                                                          AND tc.table_name = col.table_name
                                                          AND tc.constraint_type = 'PRIMARY KEY')
    LEFT JOIN information_schema.key_column_usage kcu ON (kcu.table_catalog = tc.table_catalog
                                                          AND kcu.table_schema = tc.table_schema
                                                          AND kcu.table_name = tc.table_name
                                                          AND kcu.column_name = col.column_name)
    LEFT JOIN pg_catalog.pg_statio_all_tables stt ON (stt.schemaname = col.table_schema
                                                      AND stt.relname = col.table_name)
    LEFT JOIN pg_catalog.pg_description pgd ON (pgd.objsubid = col.ordinal_position
                                                AND pgd.objoid = stt.relid)
  WHERE col.table_schema = '%s'
    AND col.table_name in (%s)
DML;

        $sql = sprintf(
            $sql,
            $this->stSchema,
            "'" . implode("', '", $this->stTabela) . "'"
        );

        $data = $this->db->carregar($sql);
        $arTabelas = array();
        foreach ($data as $arResultado) {
            $table_name = $arResultado["table_name"];
            unset($arResultado["table_name"]);
            $arTabelas[$table_name][] = $arResultado;
        }
        return $arTabelas;
    }

    public function getPrimaryKeys ($stTabela)
    {
        //foreach
        $sql = " SELECT tc.table_schema, tc.table_name, kc.column_name
                    FROM information_schema.table_constraints tc
                      JOIN information_schema.key_column_usage kc ON (kc.table_name = tc.table_name AND kc.table_schema = tc.table_schema)
                    WHERE tc.constraint_type = 'PRIMARY KEY'
                    AND kc.position_in_unique_constraint IS NULL
                    AND tc.table_schema = '%s'
                    AND tc.table_name = '%s'
                    ORDER BY tc.table_schema, tc.table_name, kc.position_in_unique_constraint
";
        $sql = sprintf($sql, $this->stSchema, $stTabela);
        $pkData = $this->db->carregar($sql);
        return is_array($pkData) ? $pkData : array();
    }

    public function getForeignKeys ($stTabela)
    {
        $sql = "SELECT r.conname, pg_catalog.pg_get_constraintdef(r.oid, true) as condef FROM pg_catalog.pg_constraint r WHERE r.conrelid = '{$this->stSchema}.{$stTabela}'::regclass AND r.contype = 'f'";
        $fkData = $this->db->carregar($sql);
        return is_array($fkData) ? $fkData : array();
    }

    public function getAttributesFull ($stTabela)
    {
        $sqlGetEntity = <<<SQL
SELECT col.column_name, col.is_nullable, col.data_type, col.character_maximum_length, kcu.constraint_name
                FROM information_schema.columns col
                LEFT JOIN information_schema.table_constraints tc
                        ON tc.table_catalog = col.table_catalog
                        AND tc.table_schema = col.table_schema
                        AND tc.table_name = col.table_name
                        AND tc.constraint_type = 'PRIMARY KEY'
                LEFT JOIN information_schema.key_column_usage kcu
                        ON kcu.table_catalog = tc.table_catalog
                        AND kcu.table_schema = tc.table_schema
                        AND kcu.table_name = tc.table_name
                        AND kcu.column_name = col.column_name
                WHERE col.table_schema = '{$this->stSchema}'
                AND col.table_name = '{$stTabela}'
SQL;

        $entity = $this->db->carregar($sqlGetEntity);
        return is_array($entity) ? $entity : array();
    }

    public function gerarArquivos ($stPrefixoClasse, $stExtensao = '.class.inc')
    {
        $tabelas = $this->getTables();

        foreach ($tabelas as $tabela => $atributos) {
            $pkData = $this->getPrimaryKeys($tabela);
            $fkData = $this->getForeignKeys($tabela);

            $modelGenerator = new ModelGenerator(array(
                'prefixoClasse' => $stPrefixoClasse . 'Model',
                'extensao'      => $stExtensao,
                'tabela'        => $tabela,
                'atributos'     => $atributos,
                'schema'        => $this->stSchema
            ));
            $modelGenerator->gerarModel($pkData, $fkData);

            $viewGenerator = new ViewGenerator(array('prefixoClasse' => $stPrefixoClasse, 'extensao' => '.php', 'tabela' => $tabela, 'atributos' => $atributos, 'schema' => $this->stSchema));
            $viewGenerator->gerarView($pkData, $fkData);

            $controllerGenerator = new ControllerGenerator(array('prefixoClasse' => $stPrefixoClasse, 'extensao' => $stExtensao, 'tabela' => $tabela, 'atributos' => $atributos, 'schema' => $this->stSchema));
            $controllerGenerator->gerarController($pkData, $fkData);
        }
    }

    public function getSchemas ()
    {
        $db = new cls_banco();
        $dml = <<<DML
SELECT DISTINCT schemaname AS schema
    FROM pg_catalog.pg_tables
    WHERE schemaname NOT IN ('pg_catalog', 'information_schema', 'pg_toast')
    ORDER BY schemaname
DML;
        return $db->carregar($dml);
    }

    public function getTablesBySchema ($idSchema = null)
    {
        $db = new cls_banco();
        $dml = "SELECT tablename AS table FROM pg_catalog.pg_tables WHERE schemaname = '%s' ORDER BY 1";
        $dml = sprintf($dml, $idSchema);
        $tabelas = $db->carregar($dml);
        $tabelas = is_array($tabelas) ? $tabelas : array();
        return $tabelas;
    }

    public function getComboTables ($idSchema = null)
    {
        $tabelas = $this->getTablesBySchema($idSchema);
        $options = '<option value="todas">-- Todas --</option>';
        foreach ($tabelas as $tabela) {
            $options .= "<option value='{$tabela['table']}'>{$tabela['table']}</option>";
        }

        echo $options;
    }
}

