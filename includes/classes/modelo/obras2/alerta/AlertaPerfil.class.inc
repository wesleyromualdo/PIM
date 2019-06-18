<?php

class AlertaPerfil extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.alerta_perfil";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("apid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'apid' => null,
        'aid' => null,
        'pflcod' => null
    );

    public function buscaAlerta()
    {
        $retorno = $this->verificaPerfil();
        if (!$retorno) return false;
        $obrids = $this->capturaObras($retorno['where']);

        if($obrids) {
            $pflcods = implode(",",$retorno['pflcod']);
            $sql = <<<DML
                SELECT array_to_string(array_agg(DISTINCT ap.aid),',')
                FROM {$this->stNomeTabela} ap
                INNER JOIN obras2.alerta a ON (ap.aid = a.aid)
                INNER JOIN obras2.alerta_obra ao ON (a.aid = ao.aid)
                WHERE ap.pflcod IN ($pflcods)
                    AND a.situacao = 'A'
                    AND ao.situacao = 'A'
                    AND ao.obrid IN ($obrids);
DML;
            $id_alertas = $this->pegaUm($sql);
            if($id_alertas) {
                $saida = array();
                $sql = <<<DML
                    SELECT
                        a.aid,
                        array_to_string(array_agg(ao.obrid||'|@|'||o.obrnome||'|@|'||mun.estuf||'|@|'||mun.mundescricao||'|@|'||tpo.tpodsc||'|@|'||ao.aoid),',,') AS obrid_list
                    FROM obras2.alerta a
                    INNER JOIN obras2.alerta_obra ao ON (a.aid = ao.aid AND ao.situacao = 'A')
                    INNER JOIN obras2.obras o ON (ao.obrid = o.obrid)
                    LEFT JOIN entidade.endereco edr ON edr.endid = o.endid
                    LEFT JOIN territorios.municipio mun ON mun.muncod = edr.muncod
                    LEFT JOIN obras2.tipologiaobra tpo ON tpo.tpoid = o.tpoid
                    WHERE a.aid IN ({$id_alertas})
                        AND ao.obrid IN ($obrids)
                    GROUP BY 1;
DML;
                return $this->carregar($sql);
            }
        }
    }

    protected function capturaObras($where)
    {
        $sql = <<<DML
            WITH  usuarioresponsabilidade AS (
                SELECT entid, pflcod, estuf, orgid, empid FROM obras2.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}'
            )
            SELECT array_to_string(array_agg(o.obrid),',')
            FROM obras2.obras o
            LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
            LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
            LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
            LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
            WHERE {$where}
DML;
        return $this->pegaUm($sql);
    }

    protected function verificaPerfil()
    {
        $arrObr = array(
            PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
            PFLCOD_EMPRESA_VISTORIADORA_GESTOR
        );

        $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

        $arrOrg = Array(PFLCOD_ADMINISTRADOR,
            PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            PFLCOD_SUPERVISOR_MEC,
            PFLCOD_GESTOR_MEC);

        $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

        $arPflcod = array();

        if (possui_perfil($arrEst)) {
            $arPflcod = array_merge($arPflcod, $arrEst);
            $where = "mun.estuf IN ( SELECT estuf FROM usuarioresponsabilidade urs WHERE
                urs.pflcod IN (" . implode(', ', $arPflcod) . ")
                AND urs.estuf = edr.estuf)";
        }

        if (possui_perfil($arrObr)) {
            $arPflcod = array_merge($arPflcod, $arrObr);
            $where = "e.empid IN ( SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                urs.pflcod IN (" . implode(', ', $arPflcod) . ")
                AND urs.empid = e.empid)";
        }

        if (possui_perfil($arrOrg)) {
            $arPflcod = array_merge($arPflcod, $arrOrg);
            $where = "e.orgid IN ( SELECT urs.orgid FROM usuarioresponsabilidade urs WHERE
                urs.pflcod IN (" . implode(', ', $arPflcod) . ")
                AND urs.orgid = e.orgid)";
        }

        if (possui_perfil($arrUni)) {
            $arPflcod = array_merge($arPflcod, $arrUni);
            $where = "e.entidunidade IN ( SELECT urs.entid FROM usuarioresponsabilidade urs WHERE
                urs.pflcod IN (" . implode(', ', $arPflcod) . ")
                AND urs.entid = e.entidunidade)";
        }

        if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

            $where = "e.empid IN ( SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                urs.empid IS NOT NULL)";
        }

        return $where ? array('where' => $where, 'pflcod' => $arPflcod) : false;

    }

    public function buscaPerfisPorAlerta($aid)
    {
        if(!$aid)
            return array();

        $sql = <<<DML
            SELECT array_to_string(array_agg(pflcod),',') as saida FROM {$this->stNomeTabela} WHERE aid = $aid;
DML;
        $saida = $this->pegaUm($sql);
        if($saida) {
            return explode(',',$saida);
        }
        return false;
    }

    public function salvarAlertaPerfil($dados, $id)
    {
        $sql = "DELETE FROM {$this->stNomeTabela} WHERE aid = $id;";
        if($dados['pflcod']){
            foreach($dados['pflcod'] as $pflcod){
                $sql .= "INSERT INTO {$this->stNomeTabela} (aid,pflcod) VALUES ($id,$pflcod);";
            }
        }
        $this->executar($sql);
        $this->commit();
    }
}