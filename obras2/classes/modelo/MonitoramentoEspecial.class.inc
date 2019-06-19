<?php

class MonitoramentoEspecial extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.monitoramento_especial";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("mesid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'mesid' => null,
        'meoid' => null,
        'usucpfinclusao' => null,
        'mesdtcadastro' => null,
        'mesdtlimite' => null,
        'mestitulo' => null,
        'mesdescricao' => null,
        'docidsituacao' => null,
        'docidresponsavel' => null,
        'messtatus' => null,
        'mesdtinclusao' => null,
        'obrid' => null,
        'rstid' => null,
        'aid' => null,
        'prazo_alerta' => null,
    );

    public function pegaHistorico($mesid)
    {
        $sql = "
                SELECT * FROM (

                   SELECT
                        u.usucpf,
                        u.usunome,
                        TO_CHAR(docdatainclusao, 'DD/MM/YYYY às HH:MI') as data,
                        '' cmddsc,
                        '' origem,
                        '' destino,
                        NULL::INTEGER esdid,
                        docdatainclusao as htddata,
                        ".TPDID_RESTRICAO_INCONFORMIDADE." as tpdid
                    FROM obras2.monitoramento_especial m
                    JOIN obras2.restricao r ON r.rstid = m.rstid
                    JOIN workflow.documento d ON d.docid = r.docid
                    JOIN seguranca.usuario u ON u.usucpf = r.usucpf
                    WHERE
                        m.messtatus = 'A'
                        AND m.mesid = $mesid

                    UNION

                    SELECT
                        u.usucpf,
                        u.usunome,
                        TO_CHAR(mesdtinclusao, 'DD/MM/YYYY às HH:MI') as data,
                        '' cmddsc,
                        '' origem,
                        '' destino,
                        NULL::INTEGER esdid,
                        mesdtinclusao as htddata,
                        NULL as tpdid
                    FROM obras2.monitoramento_especial m
                    JOIN seguranca.usuario u ON u.usucpf = m.usucpfinclusao
                    WHERE
                        m.messtatus = 'A'
                        AND m.mesid = $mesid

                    UNION

                  SELECT
                        u.usucpf,
                        u.usunome,
                        TO_CHAR(h.htddata, 'DD/MM/YYYY às HH:MI') as data,
                        c.cmddsc,
                        eo.esddsc origem,
                        ed.esddsc destino,
                        ed.esdid,
                        h.htddata,
                        d.tpdid
                    FROM obras2.monitoramento_especial m
                    JOIN obras2.restricao r ON r.rstid = m.rstid
                    JOIN workflow.documento d ON d.docid = r.docid
                    JOIN workflow.historicodocumento h ON h.docid = d.docid
                    LEFT JOIN workflow.comentariodocumento c ON c.hstid = h.hstid
                    JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
                    JOIN workflow.estadodocumento eo ON a.esdidorigem = eo.esdid
                    JOIN workflow.estadodocumento ed ON a.esdiddestino = ed.esdid
                    JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                    WHERE
                        m.messtatus = 'A'
                        AND m.mesid = $mesid


                    UNION

                    SELECT
                        u.usucpf,
                        u.usunome,
                        TO_CHAR(h.htddata, 'DD/MM/YYYY às HH:MI') as data,
                        c.cmddsc,
                        eo.esddsc origem,
                        ed.esddsc destino,
                        ed.esdid,
                        h.htddata,
                        d.tpdid
                    FROM obras2.monitoramento_especial m
                    JOIN workflow.documento d ON d.docid = m.docidsituacao
                    JOIN workflow.historicodocumento h ON h.docid = d.docid
                    LEFT JOIN workflow.comentariodocumento c ON c.hstid = h.hstid
                    JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
                    JOIN workflow.estadodocumento eo ON a.esdidorigem = eo.esdid
                    JOIN workflow.estadodocumento ed ON a.esdiddestino = ed.esdid
                    JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                    WHERE
                        m.messtatus = 'A'
                        AND m.mesid = $mesid

                    UNION

                    SELECT
                        u.usucpf,
                        u.usunome,
                        TO_CHAR(h.htddata, 'DD/MM/YYYY às HH:MI') as data,
                        c.cmddsc,
                        eo.esddsc origem,
                        ed.esddsc destino,
                        ed.esdid,
                        h.htddata,
                        d.tpdid
                    FROM obras2.monitoramento_especial m
                    JOIN workflow.documento d ON d.docid = m.docidresponsavel
                    JOIN workflow.historicodocumento h ON h.docid = d.docid
                    LEFT JOIN workflow.comentariodocumento c ON c.hstid = h.hstid
                    JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
                    JOIN workflow.estadodocumento eo ON a.esdidorigem = eo.esdid
                    JOIN workflow.estadodocumento ed ON a.esdiddestino = ed.esdid
                    JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                    WHERE
                        m.messtatus = 'A'
                        AND m.mesid = $mesid

                ) as f
                ORDER BY htddata ASC
        ";

        return $this->carregar($sql);
    }

    public function enviaAlerta($mesid)
    {
        $this->carregarPorId($mesid);

        if($this->rstid && !$this->aid){
            $restricao = new Restricao($this->rstid);
            $alerta = new Alerta();
            $alertaPerfilFiscal = new AlertaPerfil();
            $alertaPerfilGestor = new AlertaPerfil();
            $obra = new Obras($this->obrid);
            $monitoramento = new MonitoramentoEspecial($mesid);

            $mensagem = "
                <p>
                    Cumprimentando-o cordialmente,
                </p>
                <p>
                    Informamos que foi criada uma nova restrição para a obra ({$obra->obrid}) {$obra->obrnome} com o seguinte conteúdo:
                </p>
                <p>
                    <b>Descrição: </b> {$restricao->rstdsc}
                </p>
                <p>
                    <b>Providência: </b> {$restricao->rstdscprovidencia}
                </p>
            ";

            $alerta->titulo = "Restrição criada para a obra ({$obra->obrid}) {$obra->obrnome}";
            $alerta->mensagem = $mensagem;
            $alerta->cabecalho = 'A';
            $alerta->registro_atividade = 'A';
            $alerta->agid = 2;
            $alerta->salvar();


            $alertaPerfilFiscal->aid = $alerta->aid;
            $alertaPerfilFiscal->pflcod = 948;
            $alertaPerfilFiscal->salvar();

            $alertaPerfilGestor->aid = $alerta->aid;
            $alertaPerfilGestor->pflcod = 946;
            $alertaPerfilGestor->salvar();


            $monitoramento->aid = $alerta->aid;
            $monitoramento->salvar();
            $this->commit();

            $email = new Email();
            $email->enviaEmailRestricaoMonitoramentoEspecial($this->obrid, $mensagem);

        }

    }
}











