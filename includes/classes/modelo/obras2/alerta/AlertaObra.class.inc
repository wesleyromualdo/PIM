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

    public function novo($obrid,$aid){
        $sql = "SELECT aid, situacao FROM obras2.alerta_obra WHERE aid = $aid AND obrid = $obrid;";
        $linha = $this->pegaLinha($sql);
        if(!$linha) {
            $sql = "INSERT INTO obras2.alerta_obra (aid,obrid,situacao) VALUES ($aid,$obrid,'A')";
            $this->executar($sql);
            $this->commit();
        } else if($linha['situacao'] == 'C') {
            $sql = "UPDATE obras2.alerta_obra SET situacao = 'A' WHERE obrid = $obrid AND aid = $aid";
            $this->executar($sql);
            $this->commit();
        }

    }

    public function removeAlerta($obrid, $aid){
        $sql = "UPDATE obras2.alerta_obra SET situacao = 'I' WHERE aid = $aid AND obrid = $obrid;";
        return $this->carregar($sql);
    }
}