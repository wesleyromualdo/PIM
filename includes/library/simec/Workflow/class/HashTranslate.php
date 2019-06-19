<?php

/**
 * Classe que cuida da geração do hashid existentes nas tabelas:
 *      - workflow.tipodocumento,
 *      - workflow.estadodocumento,
 *      - workflow.acaoestadodoc.
 *
 * @example
 * <code>
 * // -- Criação do id do tipo de documento a partir do hashid
 * $tpdhash = 'd4dcf9158821629e547029a03b47b78d';
 * $tpdid = (new Siemc_Workflow_HashTranslate('workflow.tipodocumento', $tpdhash));
 *
 *
 * // -- Criação do id do estado de documento a partir do hashid
 * $esdhash = 'f50e2e87aacd61e9322b4d93e02d6709';
 * $esdid = (new Siemc_Workflow_HashTranslate('workflow.estadodocumento', $esdhash));
 *
 *
 * // -- Criação do id da ação do estado de documento a partir do hashid
 * $aedhash = '85d6100c46e4caf3d2c7e2b33f25bb5b';
 * $aedid = (new Siemc_Workflow_HashTranslate('workflow.acaoestadodoc', $aedhash));
 * </code>
 *
 * Class Simec_Workflow_HashTranslate
 */
class Simec_Workflow_HashTranslate {
    public $id;

    public function __construct($table, $hashid){
        $this->id = $this->hashToId($table, $hashid);
    }

    /**
     * Fução que retorna o ID da tabela a partir do hashid.
     *
     * @param $table
     * @param $hashid
     * @return bool|mixed|NULL|string
     * @throws Exception
     */
    private function hashToId($table, $hashid){
        global $db;
        if(!isset($_SESSION[$hashid])) {
            $sqlPk = "SELECT
					a.attname AS pk
					FROM
						pg_index i JOIN pg_attribute a ON
						a.attrelid = i.indrelid
						AND a.attnum = any(i.indkey)
					WHERE
						i.indrelid = '{$table}'::regclass
						AND i.indisprimary
					LIMIT 1";
            $pk = $db->pegaUm($sqlPk);
            $prefixo = substr($pk,0,3);
            $sqlId = "SELECT " . $pk . " FROM {$table} WHERE {$prefixo}hash = '{$hashid}' LIMIT 1";
            $id = $db->pegaUm($sqlId);
            if (empty($id)) {
                throw new \Exception("Nenhum ID encontrado para o HashID: " . $hashid);
            }
            $_SESSION[$hashid] = $id;
            return $id;
        } else {
            return $_SESSION[$hashid];
        }
    }
}