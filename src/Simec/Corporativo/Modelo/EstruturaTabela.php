<?php
namespace Simec\Corporativo\Modelo;

class EstruturaTabela extends \Simec\AbstractModelo
{
    
    public function lerDefinicaoTabela($esquema, $tabela = null){
        $sql = <<<sql
			select distinct
			schemaname as esquema,
			cols.table_name as nome,
			(select pg_catalog.obj_description(oid) from pg_catalog.pg_class c where c.relname=cols.table_name limit 1) as descricao
			from information_schema.columns cols
			inner join pg_tables tab on cols.table_name = tab.tablename
			where
				schemaname <> 'pg_catalog' and
				schemaname <> 'information_schema' and
                schemaname = '{$esquema}' 
sql;
        if($tabela){
            $sql.=" and cols.table_name = '{$tabela}' ";
        }
        return $this->carregarColecao($sql, new \Simec\Colecao(), new \Simec\Corporativo\Dado\EstruturaTabela());
    }

    public function lerDefinicaoCampos($esquema, $tabela)
    {
        $sql = <<<sql
SELECT DISTINCT tabela.*,
                CASE
                    WHEN tabela."tipoDado" <> 'numerico' THEN tamanho
                    ELSE tamanho/65536
                END AS tamanho,
                pk.campo_pk as "campoPk",
                fk.constraint as "constraint",
                fk.esquema_fk as "esquemaFk",
                fk.tabela_fk as "tabelaFk",
                fk.campo_fk as "campoFk",
                uk.unique_key as "uniqueKey"
FROM
  (SELECT n.nspname AS esquema,
          c.relname AS tabela,
          a.attname AS campo,
          CASE
              WHEN a.attnotnull = 't' THEN 'not null'
              WHEN a.attnotnull = 'f' THEN ''
          END AS obrigatorio,
          format_type(t.oid, NULL) AS tipo,
          CASE
              WHEN format_type(t.oid, NULL) = 'bigint' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'bigserial' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'bit' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'bit varying' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'boolean' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'character varying' THEN 'texto'
              WHEN format_type(t.oid, NULL) = 'character' THEN 'texto'
              WHEN format_type(t.oid, NULL) = 'date' THEN 'data'
              WHEN format_type(t.oid, NULL) = 'double precision' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'integer' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'interval' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'money' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'numeric' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'real' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'smallint' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'serial' THEN 'numerico'
              WHEN format_type(t.oid, NULL) = 'text' THEN 'texto'
              WHEN format_type(t.oid, NULL) = 'time with time zone' THEN 'data'
              WHEN format_type(t.oid, NULL) = 'timestamp' THEN 'data'
              WHEN format_type(t.oid, NULL) = 'timestamp with time zone' THEN 'data'
              WHEN format_type(t.oid, NULL) = 'timestamp without time zone' THEN 'data'
              ELSE coalesce(format_type(t.oid, NULL), 'tdata')
          END AS "tipoDado",
          CASE
              WHEN a.attlen >= 0 THEN a.attlen
              ELSE (a.atttypmod-4)
          END AS tamanho,
          cm.description AS descricao
   FROM pg_attribute a
   INNER JOIN pg_class c ON a.attrelid = c.oid
   INNER JOIN pg_namespace n ON c.relnamespace = n.oid
   INNER JOIN pg_type t ON a.atttypid = t.oid
   LEFT JOIN pg_description cm ON a.attrelid::oid = cm.objoid::oid
   AND attnum = cm.objsubid
   WHERE c.relkind = 'r' -- no indices
AND n.nspname NOT LIKE 'pg\\_%' -- no catalogs
AND n.nspname != 'information_schema' -- no information_schema
AND a.attnum > 0 -- no system att's
AND NOT a.attisdropped -- no dropped columns
) AS tabela
LEFT JOIN 
  (SELECT pg_namespace.nspname AS esquema, 
          pg_class.relname AS tabela, 
          pg_attribute.attname AS campo_pk
   FROM pg_class
   JOIN pg_namespace ON pg_namespace.oid=pg_class.relnamespace
   AND pg_namespace.nspname NOT LIKE E'pg_%'
   JOIN pg_attribute ON pg_attribute.attrelid=pg_class.oid
   AND pg_attribute.attisdropped='f'
   JOIN pg_index ON pg_index.indrelid=pg_class.oid
   AND pg_index.indisprimary='t'
   AND (pg_index.indkey[0]=pg_attribute.attnum
        OR pg_index.indkey[1]=pg_attribute.attnum
        OR pg_index.indkey[2]=pg_attribute.attnum
        OR pg_index.indkey[3]=pg_attribute.attnum
        OR pg_index.indkey[4]=pg_attribute.attnum
        OR pg_index.indkey[5]=pg_attribute.attnum
        OR pg_index.indkey[6]=pg_attribute.attnum
        OR pg_index.indkey[7]=pg_attribute.attnum
        OR pg_index.indkey[8]=pg_attribute.attnum
        OR pg_index.indkey[9]=pg_attribute.attnum)) AS pk ON (tabela.esquema = pk.esquema
                                                              AND tabela.tabela = pk.tabela
                                                              AND tabela.campo = pk.campo_pk)
LEFT JOIN 
  (SELECT n.nspname AS esquema, 
          cl.relname AS tabela, 
          a.attname AS campo, 
          ct.conname AS CONSTRAINT, 
          nf.nspname AS esquema_fk, 
          clf.relname AS tabela_fk, 
          af.attname AS campo_fk --pg_get_constraintdef(ct.oid) AS criar_sql
FROM pg_catalog.pg_attribute a
   JOIN pg_catalog.pg_class cl ON (a.attrelid = cl.oid
                                   AND cl.relkind= 'r')
   JOIN pg_catalog.pg_namespace n ON (n.oid = cl.relnamespace)
   JOIN pg_catalog.pg_constraint ct ON (a.attrelid = ct.conrelid
                                        AND ct.confrelid != 0
                                        AND ct.conkey[1] = a.attnum)
   JOIN pg_catalog.pg_class clf ON (ct.confrelid = clf.oid
                                    AND clf.relkind = 'r')
   JOIN pg_catalog.pg_namespace nf ON (nf.oid = clf.relnamespace)
   JOIN pg_catalog.pg_attribute af ON (af.attrelid = ct.confrelid
                                       AND af.attnum = ct.confkey[1])) AS fk ON (tabela.esquema = fk.esquema
                                                                                 AND tabela.tabela = fk.tabela
                                                                                 AND tabela.campo = fk.campo)
LEFT JOIN 
  ( SELECT ic.relname AS index_name, 
           bn.nspname AS SCHEMA_NAME, 
           bc.relname AS tab_name, 
           ta.attname AS COLUMN_NAME, 
           i.indisunique AS unique_key, 
           i.indisprimary AS primary_key 
   FROM pg_class bc, 
        pg_namespace bn, 
        pg_class ic, 
        pg_index i, 
        pg_attribute ta, 
        pg_attribute ia 
   WHERE bc.oid = i.indrelid 
     AND ic.oid = i.indexrelid 
     AND ia.attrelid = i.indexrelid 
     AND ta.attrelid = bc.oid 
     AND ta.attrelid = i.indrelid 
     AND ta.attnum = i.indkey[ia.attnum-1] 
     AND bn.oid = bc.relnamespace 
     AND i.indisunique = TRUE 
     AND i.indisprimary = FALSE) AS uk ON ( tabela.esquema = uk.schema_name 
                                                                        AND tabela.tabela = uk.tab_name 
                                                                        AND tabela.campo = uk.column_name ) 
WHERE tabela.esquema || '.' || tabela.tabela = '{$esquema}.{$tabela}'
ORDER BY tabela.esquema,
         tabela.tabela,
         pk.campo_pk,
         fk.campo_fk
sql;
        return $this->carregarColecao($sql, new \Simec\Colecao(), new \Simec\Corporativo\Dado\EstruturaCampo());
    }
}
