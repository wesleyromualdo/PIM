<?php

class AlertaObra extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.alerta_obra";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("aoid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'aoid' => null,
        'aid' => null,
        'obrid' => null,
        'situacao' => null
    );

    public function buscaAlertaObras($id_alerta)
    {
        if(!$id_alerta) return array();
        $sql = "SELECT ao.aoid, ao.obrid, ao.situacao, obr.obrnome
        FROM obras2.alerta_obra ao
        INNER JOIN obras2.obras obr ON (ao.obrid = obr.obrid)
        WHERE ao.aid = ". $id_alerta."
            AND ao.situacao != 'I'
        ORDER BY  ao.obrid";
        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public function salvarAlertaObra($dados)
    {
        $existe = false;
        if($dados['obrid_out']) {
            $existe = true;
            $obridOut = implode(',',$dados['obrid_out']);
            $sql = "UPDATE obras2.alerta_obra SET situacao = 'I' WHERE aid = {$dados['aid']} AND obrid IN ($obridOut);";
        }
        if($dados['obrid']) {
            foreach($dados['obrid'] as $obra) {
                if($existe) {
                    if(in_array($obra,$dados['obrid_out'])) {
                        $sql .= "UPDATE obras2.alerta_obra SET situacao = 'A' WHERE aid = {$dados['aid']} AND obrid = $obra;";
                        continue;
                    }
                }
                $sql .= "INSERT INTO obras2.alerta_obra (aid,obrid,situacao) VALUES ({$dados['aid']},$obra,'A'); ";
            }
        }
        if($sql) {
            $this->executar($sql);
            $this->commit();
        }
    }

    public function salvarAlerta($dados)
    {
        try{
            foreach($dados['aoid'] as $key => $id) {
                if($dados['ra'] == 'A'){
                    $this->registraAtividade($dados['obrid'][$key], $dados['aid']);
                }

                $alertaObra = new AlertaObra();
                $alertaObra->aoid = $id;
                $alertaObra->situacao = 'C';
                $alertaObra->salvar();
            }
            $this->commit();
            return true;
        } catch(Exception $e) {
            return false;
        }
    }

    /**
     * Registra na tabela de atividades do envio do e-mail
     */
    protected function registraAtividade($obrid, $id_alerta) {
        $rgaid = $this->pegaUm("select nextval('obras2.registroatividade_rgaid_seq')");
        $tituloNotificacao = $this->pegaUm("select titulo from obras2.alerta where aid = ".$id_alerta);

        $arDado = array();
        $arDado['rgaid'] = $rgaid;
        $arDado['obrid'] = $obrid;
        $arDado['rgaautomatica'] = 'true';
        $arDado['rgadscsimplificada'] = 'Leitura da Notificação '.$tituloNotificacao.' N°'.str_pad($id_alerta,7,'0',STR_PAD_LEFT);
        $arDado['rgadsccompleta'] = 'A Notificação '.$tituloNotificacao.' N°'.str_pad($id_alerta,7,'0',STR_PAD_LEFT).' foi lido na data '.date('d/m/Y \à\s H:i').' pelo usuário ('.mascaraglobal($_SESSION['usucpf'],'###.###.###-##').') '. $_SESSION['usunome'].'.';
        $arDado['arqid'] = 'NULL';

        $sql = "INSERT INTO obras2.registroatividade (rgaid, arqid, obrid, rgaautomatica, rgadscsimplificada, rgadsccompleta) VALUES (
                  {$arDado['rgaid']},
                  {$arDado['arqid']},
                  {$arDado['obrid']},
                  {$arDado['rgaautomatica']},
                  '{$arDado['rgadscsimplificada']}',
                  '{$arDado['rgadsccompleta']}'
                  )";
        $this->executar($sql);
    }

    /**
     * Cria um novo alerta de obra.
     *
     * A criação pode ser feita tanto inserindo um novo registro na tabela, quanto ativando
     * um item inativo, ou clicado.
     *
     * @param int $obrid ID da obra.
     * @param int $aid ID do alerta.
     * @return bool
     */
    public function novo($obrid, $aid)
    {
        $sql = "SELECT aid, situacao FROM obras2.alerta_obra WHERE aid = {$aid} AND obrid = {$obrid};";
        $linha = $this->pegaLinha($sql);
        if (!$linha) {
            $sql = "INSERT INTO obras2.alerta_obra (aid,obrid,situacao) VALUES ({$aid}, {$obrid}, 'A')";
            return $this->executar($sql);
        } elseif (($linha['situacao'] == 'C') || ('I' == $linha['situacao'])) {
            $sql = "UPDATE obras2.alerta_obra SET situacao = 'A' WHERE obrid = {$obrid} AND aid = {$aid}";
            return $this->executar($sql);
        }
    }

    /**
     * Remove um alerta de obra.
     *
     * Essa remoção é feita apenas de forma lógica, alterando o valor de situação para I.
     *
     * @param int $obrid ID da obra.
     * @param int $aid ID do alerta.
     * @return bool
     */
    public function removeAlerta($obrid, $aid){
        $sql = "UPDATE obras2.alerta_obra SET situacao = 'I' WHERE aid = {$aid} AND obrid = {$obrid};";
        return $this->executar($sql);
    }

    /**
     * Retorna os dados ou a query de obras para associação ao alerta.
     *
     * As opções disponíveis são:
     * * apenasid: A query retorna apenas os ids das obras.
     * * sql: Retorna apenas a query.
     *
     * @param array $param Parâmetros de restrição da busca.
     * @param array $opcoes Opções de modificação do funcionamento do método.
     * @return string|array
     */
    public function retornaListaDeObrasFiltradas(array $param = [], array $opcoes = [])
    {
        $where['strid'] = " o.obridpai IS NULL AND o.obrstatus = 'A' ";
        // verificando se é undidade ou unidade gestora
        if ($param['strid'] && $param['strid'][0] != 'null') {
            $where['strid'] = "str.strid IN(" . implode(',', $param['strid']) . ")";
        }
        if ($param['tpoid'] && $param['tpoid'][0] != 'null') {
            $where['tpoid'] = "o.tpoid IN ('" . implode("', '", $param['tpoid']) . "')";
        }
        if ($param['prfid'] && $param['prfid'][0] != 'null') {
            $where['prfid'] = "e.prfid IN(" . implode(',', $param['prfid']) . ")";
        }
        if ($param['tooid'] && $param['tooid'][0] != 'null') {
            $where['tooid'] = "o.tooid IN(" . implode(',', $param['tooid']) . ")";
        }
        if ($param['estuf'] && $param['estuf'][0] != 'null') {
            $where['estuf'] = "mun.estuf IN ('" . implode("', '", $param['estuf']) . "')";
        }
        if ($param['muncod'] && $param['muncod'][0] != 'null') {
            $where['muncod'] = "mun.muncod IN ('" . implode("', '", $param['muncod']) . "')";
        }
        if ($param['empesfera']) {
            $where['empesfera'] = "e.empesfera IN('{$param['empesfera']}')";
        }
        if ($param['processo']) {
            $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')";
        }
        if ($param['ano_processo']) {
            $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$param['ano_processo']}'";
        }
        if ($param['convenio']) {
            $where['convenio'] = "p_conv.termo_convenio = '{$param['convenio']}'";
        }
        if ($param['ano_convenio']) {
            $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$param['ano_convenio']}'";
        }
        if (isset($param['obrid']) && !empty($param['obrid'])) {
            $where['obrid'] = "o.obrid = {$param['obrid']}";
        }

        if (empty($where)) {
            $where = '';
        } else {
            $where = 'WHERE ' . implode(' AND ', $where);
        }

        $select = 'o.obrid, o.obrid AS id, o.obrnome';
        if ($opcoes['apenasid']) {
            $select = 'o.obrid';
        }

        $sql_lista = <<<DML
SELECT {$select}
  FROM obras2.obras o
    LEFT JOIN obras2.empreendimento e ON e.empid = o.empid
    LEFT JOIN entidade.endereco edr ON edr.endid = o.endid
    LEFT JOIN territorios.municipio mun ON mun.muncod = edr.muncod
    LEFT JOIN workflow.documento d ON d.docid = o.docid
    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
    LEFT JOIN obras2.situacao_registro_documento srd ON srd.esdid = d.esdid
    LEFT JOIN obras2.situacao_registro str ON str.strid = srd.strid
    LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid
  {$where}
  ORDER BY o.obrid
DML;
  
  if ($opcoes['sql']) {
            return $sql_lista;
        }

        $dados = $this->carregar($sql_lista);
        return (empty($dados)?[]:$dados);
    }
}
