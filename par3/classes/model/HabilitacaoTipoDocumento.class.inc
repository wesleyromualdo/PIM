<?php
/**
 * Classe de mapeamento da entidade par3.tipodocumento.
 *
 * @version $Id$
 * @since 2018.06.26
 */

/**
 * Par3_Model_HabilitacaoTipoDocumento: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Thiago Tasca Barbosa <thiago.barbosa@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_HabilitacaoTipoDocumento($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_TipoDocumento($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 */
class Par3_Model_HabilitacaoTipoDocumento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'habilita.tipodocumento';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'tdcid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
//    protected $arChaveEstrangeira = array(
//        'usucpfinclusao'  => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
//        'usucpfalteracao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
//    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'tdcid' => null,
        'tdcnome' => null,
        'tdcdica' => null,
        'tdcvigencia' => null,
        'tdcstatus' => null
    );

    /**
     * Validators.Par3_Model_HabilitacaoTipoDocumento
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
//    public function getCamposValidacao($dados = array())
//    {
//        return array(
//            'tpeid' => array('Digits'),
//            'tpenome' => array('Digits'),
//            'tpehabilitacao' => array('Digits'), //array('allowEmpty' => true),
//            'tpestatus' => array('Digits'),
//        );
//    }
    
    public function montarSQLSimples($arrPost)
    {
        $tpenome = sanitizar_string_pesquisa($arrPost['tdcnome']);

        $where = "WHERE tdcstatus = 'A'";

        $where .= $arrPost['tdcnome']?" AND upper(removeacento(tdcnome)) LIKE upper(removeacento('%{$tdcnome}%'))":'';
//        $where .= $arrPost['tpehabilitacao']?" AND tpehabilitacao = '{$arrPost['tpehabilitacao']}'": "AND tpehabilitacao IN ('S', 'N')";
        $where .= $arrPost['tdcid'] && validarInteiro($arrPost['tdcid']) ?" AND tdcid = {$arrPost['tdcid']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                tdcid AS id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                tdcid AS codigo,
                tdcnome AS descricao,
                case when tdcvigencia = 'S' then 'Sem Vigência' else 
                    case when tdcvigencia = 'O' then 'Obrigatório' else 'Não Obrigatório' end 
                end  AS vigencia,
                tdcdica AS dica
                FROM {$this->stNomeTabela}
                $whereSituacao
                {$where}
                {$orderBy}
                ";
//                 ver($sql);
        return $sql;
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

    public function recuperarPorTipoDocumento($tdcid)
    {
        return $this->recuperarTodos('*',array("tdcid = {$tdcid}"));
    }
    
    
    public function pegarSelectCombo($tdcid = null)
    {
        if($tdcid){
            $where = ' AND tdcid not in ('.$tdcid.') ';
        }
        $sql = "SELECT
                  tdcid as codigo,
                  tdcnome as descricao
                FROM {$this->stNomeTabela}
                WHERE
                tdcstatus = 'A'".$where;
                
        return $sql;
        
    }
}
