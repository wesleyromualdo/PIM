<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

class Par3_Model_TipoDocumentoAnexo extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.tipo_documento_anexo';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = [
        'doaid',
    ];

    protected $arChaveEstrangeira = [
        'arqid'
    ];

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = [
        'doaid' => null,
        'doadescricao' => null,
        'doatxtajuda' => null,
        'doadetalharitem' => null,
        'doadesembolso' => null,
        'doagerarpendencia' => null,
        'doadocumentofnde' => null,
        'doaanalise' => null,
        'doaqtdmaxima' => null,
        'doatxtinstrucao' => null,
        'doasituacao' => null,
        'arqid' => null,
        'doastatus' => null,
    ];

    public function antesSalvar()
    {
        $this->doadetalharitem = ($this->doadetalharitem === 'on') ? 'TRUE' : 'FALSE';
        $this->doadesembolso = ($this->doadesembolso === 'on') ? 'TRUE' : 'FALSE';
        $this->doagerarpendencia = ($this->doagerarpendencia === 'on') ? 'TRUE' : 'FALSE';
        $this->doadocumentofnde = ($this->doadocumentofnde === 'on') ? 'TRUE' : 'FALSE';
        $this->doaanalise = ($this->doaanalise === 'on') ? 'TRUE' : 'FALSE';
        return parent::antesSalvar();
    }

    public function removerExtensoes($doaid)
    {
        $sql = 'DELETE FROM par3.tipo_documento_anexo_extensao WHERE doaid = ' . $doaid;
        return $this->executar($sql);
    }

    public function removerTipoAtendimento($doaid)
    {
        $sql = 'DELETE FROM par3.tipo_atendimento_documentos_anexos WHERE doaid = ' . $doaid;
        return $this->executar($sql);
    }

    public function salvarExtensoes($doaid, array $extensoes)
    {
        foreach ($extensoes as $extid) {
            if (!empty($extid) && $extid != 't') {
                $sql = sprintf(
                    'INSERT INTO par3.tipo_documento_anexo_extensao (extid, doaid) VALUES (%d, %d)',
                    $extid,
                    $doaid
                );
                $this->executar($sql);
            }
        }
    }

    public function salvarTipoAtendimento($doaid, array $tipoAtendimento)
    {
        foreach ($tipoAtendimento as $tpaid) {
            if (!empty($tpaid) && $tpaid != 't') {
                $sql = sprintf(
                    'INSERT INTO par3.tipo_atendimento_documentos_anexos (tpaid, doaid) VALUES (%d, %d)',
                    $tpaid,
                    $doaid
                );
                $this->executar($sql);
            }
        }
    }

    public function recuperarPorId($doaid){
        $sql = "SELECT DISTINCT 
                       doadescricao,
                       doatxtajuda,
                       doadetalharitem,
                       doadesembolso,
                       doagerarpendencia,
                       doadocumentofnde,
                       doaqtdmaxima,
                       doaanalise,
                       doatxtinstrucao,
                       doasituacao,
                       arqid,
                       tpadesc,
                       array_to_string(
                           ARRAY(
                              SELECT extdesc
                              FROM par3.tipo_documento_anexo_extensao as tdae
                                INNER JOIN extensao_arquivo ext ON (tdae.extid = ext.extid)
                              WHERE tdae.doaid = tda.doaid
                            ), ', '
                        ) AS extensoes
                 FROM par3.tipo_documento_anexo doa
                 LEFT JOIN par3.tipo_atendimento_documentos_anexos tad ON tad.doaid = doa.doaid
                 LEFT JOIN par3.tipo_atendimento tpa ON tpa.tpaid = tad.tpaid
                 LEFT JOIN par3.tipo_documento_anexo_extensao tda ON tda.doaid = doa.doaid
                 LEFT JOIN extensao_arquivo ext ON tda.extid = ext.extid
                 WHERE doa.doaid = {$doaid}
                 ";
        return $this->pegaLinha($sql);
    }
}
