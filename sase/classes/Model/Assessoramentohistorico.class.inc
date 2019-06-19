<?php
/**
 * Classe de mapeamento da entidade sase.assessoramento_historico.
 *
 * @version $Id$
 * @since 2017.09.28
 */

/**
 * Sase_Assessoramento_HistoricoModel_Assessoramento_historico: Histórico dos arquivos Lei PNE e Termo de Adesão - Município.
 *
 * @package Sase\Assessoramento
 * @uses Simec\Db\Modelo
 * @author Marilúcia Cardozo De Queiroz <marilucia.queiroz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Sase_Assessoramento_HistoricoModel_Assessoramento_historico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Sase_Assessoramento_HistoricoModel_Assessoramento_historico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $assid 
 * @property string $estuf 
 * @property string $muncod 
 * @property int $stacod 
 * @property string $usucpf 
 * @property int $docid 
 * @property int $assleipne 
 * @property int $assadesao 
 * @property \Datetime(Y-m-d H:i:s) $hasdtacao 
 * @property \Datetime(Y-m-d H:i:s) $hasdtcriacao 
 * @property string $hasacao 
 * @property string $hascpf 
 * @property int $hasid  - default: nextval('sase.assessoramento_historico_hasid_seq'::regclass)
 */
class Sase_Assessoramento_Historico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.assessoramento_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hasid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'assid' => array('tabela' => 'sase.assessoramento', 'pk' => 'assid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'assid' => null,
        'estuf' => null,
        'muncod' => null,
        'stacod' => null,
        'usucpf' => null,
        'docid' => null,
        'assleipne' => null,
        'assadesao' => null,
        'hasdtacao' => null,
        'hasdtcriacao' => null,
        'hasacao' => null,
        'hascpf' => null,
        'hasid' => null,
    );
    
    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'assid' => array('allowEmpty' => true,'Digits'),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'stacod' => array('allowEmpty' => true,'Digits'),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'docid' => array('allowEmpty' => true,'Digits'),
            'assleipne' => array('allowEmpty' => true,'Digits'),
            'assadesao' => array('allowEmpty' => true,'Digits'),
            'hasdtacao' => array(),
            'hasdtcriacao' => array(),
            'hasacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'hascpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'hasid' => array('Digits'),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }
    
    /**
     * MontaListaHistorico para a função monta_lista da classe_simec.
     *
     * @param array $id, $uf, $codigoMunicipio
     * @return table com registros (upload de arquivos) alterados na tela Assistência Técnica Município
     */
    public function montaListaHistorico( $assid, $estuf, $muncod ){
        $this->arAtributos['assid'] = $assid;
            
        $assleipne = "'<a style=\"cursor:pointer\" onclick=\"downloadAnexo( \'assleipne\',' || ah.assleipne || ' )\"><img src=\"/imagens/sase/direction.png\" title=\"Download Lei PNE - '|| pa2.arqdescricao ||' \" /></a>'";
        $assadesao = "'<a style=\"cursor:pointer\" onclick=\"downloadAnexo(\'assadesao\',' || ah.assadesao || '  )\"><img src=\"/imagens/sase/direction.png\" title=\"Download Adesão - '|| pa1.arqdescricao ||' \"/></a>'";
                
        $sql = "
            SELECT
                to_char( ah.hasdtacao, 'DD/MM/YY HH24:MI:SS' ) as hasdtacao,
                TO_CHAR(ah.hascpf::BIGINT, '000\"-\"000\"-\"000\"-\"00')  as hascpf,
                u.usunome,
                CASE
                  when ah.assleipne is not null then {$assleipne}
                  else ' -- '
                END as assleipne,
                CASE
                  when ah.assadesao is not null then {$assadesao}
                  else ' -- '
                END as assadesao
            FROM " . $this->stNomeTabela . " ah
            JOIN seguranca.usuario u ON u.usucpf = ah.hascpf
            JOIN public.arquivo pa1 ON pa1.arqid = ah.assadesao
            JOIN public.arquivo pa2 ON pa2.arqid = ah.assleipne
            WHERE assid = ".$assid." AND ah.estuf = '".$estuf."' AND ah.muncod = '".$muncod."'";
        return $sql;
    }

}
